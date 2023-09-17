/**
 * @file    symboltable.c
 * @brief   A symbol table for AMPL-2023.
 * @author  W.H.K. Bester (whkbester@cs.sun.ac.za)
 * @date    2023-07-06
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "boolean.h"
#include "error.h"
#include "hashtable.h"
#include "symboltable.h"
#include "valtypes.h"
#include "token.h"

/* --- global static variables ---------------------------------------------- */

static HashTab *table, *saved_table;
/* TODO: Nothing here, but note that the next variable keeps a running count of
 * the number of variables in the current symbol table.  It will be necessary
 * during code generation to compute the size of the local variable array of a
 * method frame in the Java virtual machine.
 */
static unsigned int curr_offset;

/* --- function prototypes -------------------------------------------------- */

static void valstr(void *key, void *p, char *str);
static void freeprop(void *p);
static unsigned int shift_hash(void *key, unsigned int size);
static int key_strcmp(void *val1, void *val2);

/* --- symbol table interface ----------------------------------------------- */

void init_symbol_table(void)
{
	saved_table = NULL;
	if ((table = ht_init(0.75f, shift_hash, key_strcmp)) == NULL)
	{
		eprintf("Symbol table could not be initialised");
	}
	curr_offset = 1;
}

Boolean open_subroutine(char *id, IDPropt *prop)
{
	if (ht_insert(table, id, prop) != EXIT_SUCCESS)
	{
		return FALSE;
	}

	saved_table = table;
	if ((table = ht_init(0.75f, shift_hash, key_strcmp)) == NULL)
	{
		eprintf("Subroutine symbol table could not be initialised");
	}
	curr_offset = 1;
	return TRUE;
}

void close_subroutine(void)
{
	ht_free(table, freeprop, freeprop);
	table = saved_table;
	curr_offset = 1;
}

Boolean insert_name(char *id, IDPropt *prop)
{
	IDPropt *oldprop;
	if (find_name(id, &oldprop))
	{
		return FALSE;
	}
	if (ht_insert(table, id, prop) != EXIT_SUCCESS)
	{
		return FALSE;
	}
	if (IS_VARIABLE((prop)->type))
	{
		prop->offset = curr_offset;
		curr_offset++;
	}
	return TRUE;
}

Boolean find_name(char *id, IDPropt **prop)
{
	*prop = ht_search(table, id);
	if (!*prop && saved_table)
	{
		*prop = ht_search(saved_table, id);
		if (*prop && !IS_CALLABLE_TYPE((*prop)->type))
		{
			*prop = NULL;
		}
	}

	return (*prop ? TRUE : FALSE);
}

int get_variables_width(void)
{
	return curr_offset;
}

void release_symbol_table(void)
{
	ht_free(table, freeprop, freeprop);
}

void print_symbol_table(void)
{
	ht_print(table, valstr);
}

/* --- utility functions ---------------------------------------------------- */

static void valstr(void *key, void *p, char *str)
{
	char *keystr = (char *)key;
	IDPropt *idpp = (IDPropt *)p;

	/* TODO: Nothing, but this should give you an idea of how to look at the
	 * content of the symbol table.
	 */

	if (IS_CALLABLE_TYPE(idpp->type))
	{
		sprintf(str, "%s@_[%s]", keystr, get_valtype_string(idpp->type));
	}
	else
	{
		sprintf(str, "%s@%d[%s]", keystr, idpp->offset,
				get_valtype_string(idpp->type));
	}
}

/* TODO: Here you should add your own utility functions, in particular, for
 * deallocation, hashing, and key comparison.  For hashing, you MUST NOT use the
 * simply strategy of summing the integer values of characters.  I suggest you
 * use some kind of cyclic bit shift hash.
 */

static void freeprop(void *p)
{
	if (p != NULL)
	{
		free(p);
	}
}

static unsigned int shift_hash(void *key, unsigned int size)
{
#ifdef DEBUG_SYMBOL_TABLE
	char *keystr = (char *)key;
	unsigned int i, hash, length;

	hash = 0;
	length = strlen(keystr);
	for (i = 0; i < length; i++)
	{
		hash += keystr[i];
	}

	return (hash % size);
#else

	char *str = (char *)key;
	unsigned int hash = 0;
	while (*str)
	{
		hash = (hash << 5) ^ *str++;
	}
	return (hash % size);

#endif /*DEBUG_SYMBOL_TABLE */
}

static int key_strcmp(void *val1, void *val2)
{
	return strcmp((char *)val1, (char *)val2);
}
