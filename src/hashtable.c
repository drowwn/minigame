/**
 * @file    hashtable.c
 * @brief   A generic hash table.
 * @author  W.H.K. Bester (whkbester@cs.sun.ac.za)
 * @date    2023-07-06
 */

#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include <string.h>
#include "hashtable.h"

#define INITIAL_DELTA_INDEX 4
#define PRINT_BUFFER_SIZE 1024

/** an entry in the hash table */
typedef struct htentry HTentry;
struct htentry
{
	void *key;		   /*<< the key                      */
	void *value;	   /*<< the value                    */
	HTentry *next_ptr; /*<< the next entry in the bucket */
};

/** a hash table container */
struct hashtab
{
	/** a pointer to the underlying table                              */
	HTentry **table;
	/** the current size of the underlying table                       */
	unsigned int size;
	/** the current number of entries                                  */
	unsigned int num_entries;
	/** the maximum load factor before the underlying table is resized */
	float max_loadfactor;
	/** the index into the delta array                                 */
	unsigned short idx;
	/** a pointer to the hash function                                 */
	unsigned int (*hash)(void *, unsigned int);
	/** a pointer to the comparison function                           */
	int (*cmp)(void *, void *);
};

/* --- function prototypes -------------------------------------------------- */

static int getsize(HashTab *ht);
static HTentry **talloc(int tsize);
static void rehash(HashTab *ht);

unsigned short delta[] = {0, 0, 1, 1, 3, 1, 3, 1, 5, 3, 3, 9, 3, 1, 3, 19, 15,
						  1, 5, 1, 3, 9, 3, 15, 3, 39, 5, 39, 57, 3, 35, 1};

#define MAX_IDX (sizeof(delta) / sizeof(short))

/* --- hash table interface ------------------------------------------------- */

HashTab *ht_init(float loadfactor,
				 unsigned int (*hash)(void *, unsigned int),
				 int (*cmp)(void *, void *))
{
	HashTab *ht;
	unsigned int i;
	if (loadfactor <= 0)
		return NULL;

	ht = (HashTab *)malloc(sizeof(HashTab));

	if (!ht)
	{
		return NULL;
	}

	ht->size = 13;
	ht->table = talloc(13);

	if (!ht->table)
	{
		free(ht);
		return NULL;
	}

	for (i = 0; i < 13; ++i)
	{
		ht->table[i] = NULL;
	}

	ht->num_entries = 0;
	ht->idx = INITIAL_DELTA_INDEX;
	ht->max_loadfactor = loadfactor;
	ht->cmp = cmp;
	ht->hash = hash;

	return ht;
}

int ht_insert(HashTab *ht, void *key, void *value)
{
	int k;
	HTentry *p, *new_entry;

	float load_factor = (float)(ht->num_entries + 1) / (float)ht->size;

	if (!ht || !key || !value)
	{
		return EXIT_FAILURE;
	}
	if (ht_search(ht, key) != NULL)
	{
		return -1;
	}
	if (load_factor > ht->max_loadfactor)
	{
		rehash(ht);
	}

	// bucket index
	k = ht->hash(key, ht->size);

	// pointer to bucket at index k
	p = ht->table[k];

	// malloc space for new entry
	new_entry = emalloc(sizeof(HTentry));

	// malloc was unsuccessful
	if (new_entry == NULL)
	{
		return HASH_TABLE_NO_SPACE_FOR_NODE;
	}

	// Initialize the new entry with the given key and value
	new_entry->key = key;
	new_entry->value = value;

	// next pointer points to current bucket
	new_entry->next_ptr = p;

	// Make new entry the current entry
	ht->table[k] = new_entry;

	ht->num_entries++;
	return EXIT_SUCCESS;
}

void *ht_search(HashTab *ht, void *key)
{
	int k;
	HTentry *p;

	if (!ht || !key)
	{
		return NULL;
	}

	k = ht->hash(key, ht->size);
	p = ht->table[k];

	while (p != NULL)
	{
		if (ht->cmp(key, p->key) == 0)
		{
			return p->value;
		}
		p = p->next_ptr;
	}

	return NULL;
}

int ht_free(HashTab *ht, void (*freekey)(void *k), void (*freeval)(void *v))
{
	unsigned int i = 0;
	HTentry *p, *q;

	if (!ht || !freekey || !freeval)
	{
		return EXIT_FAILURE;
	}

	/* free the nodes in the buckets */
	for (i = 0; i < ht->size; ++i)
	{
		p = ht->table[i];
		while (p != NULL)
		{
			q = p;
			p = p->next_ptr;
			freekey(q->key);
			freeval(q->value);
			free(q);
		}
	}

	/* free the table and container */

	free(ht->table);
	free(ht);
	return EXIT_SUCCESS;
}

void ht_print(HashTab *ht, void (*keyval2str)(void *k, void *v, char *b))
{
	unsigned int i;
	HTentry *p;
	char buffer[PRINT_BUFFER_SIZE];

	/* TODO: This function is complete and useful for testing, but you have to
	 * write your own keyval2str function if you want to use it.
	 */

	if (ht && keyval2str)
	{
		for (i = 0; i < ht->size; i++)
		{
			printf("bucket[%2i]", i);
			for (p = ht->table[i]; p != NULL; p = p->next_ptr)
			{
				keyval2str(p->key, p->value, buffer);
				printf(" --> %s", buffer);
			}
			printf(" --> NULL\n");
		}
	}
}

/* --- utility functions ---------------------------------------------------- */

/* TODO: I suggest completing the following helper functions for use in the
 * global functions ("exported" as part of this unit's public API) given above.
 * You may, however, elect not to use them, and then go about it in your own way
 * entirely.  The ball is in your court, so to speak, but remember: I have
 * suggested using these functions for a reason -- they should make your life
 * easier.
 */

static int getsize(HashTab *ht)
{
	if (ht->idx >= MAX_IDX)
	{
		return -1;
	}
	else
	{
		ht->idx++;
	}
	unsigned int new_size = (1 << ht->idx) - delta[ht->idx];
	return new_size;
}

static HTentry **talloc(int tsize)
{
	return (HTentry **)emalloc(tsize * sizeof(HTentry *));
}

static void rehash(HashTab *ht)
{

	unsigned int new_size = getsize(ht);
	unsigned int i, nb;
	HTentry **new_table = talloc(new_size);
	HTentry *p, *q;

	if (new_table == NULL)
	{
		return;
	}

	// Initialize new table
	for (i = 0; i < new_size; ++i)
	{
		new_table[i] = NULL;
	}

	// Move entries to the new table
	for (i = 0; i < ht->size; ++i)
	{
		p = ht->table[i];
		while (p != NULL)
		{
			// save next entry
			q = p->next_ptr;

			// calculate new bucket index
			nb = ht->hash(p->key, new_size);

			// next pointer of current bucket <- next pointer of new bucket
			p->next_ptr = new_table[nb];

			// new bucket in new table <- current bucket
			new_table[nb] = p;

			// move to next entry
			p = q;
		}
	}

	// Free the old table
	free(ht->table);
	// Update the hash table structure
	ht->table = new_table;
	ht->size = new_size;
}
