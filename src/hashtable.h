/**
 * @file    hashtable.h
 *
 * A hash table implementation that can store arbitrary keys and
 * values.  This implementation does not support the insertion or retrieval of
 * <code>NULL</code> keys or values.
 *
 * @author  W.H.K. Bester (whkbester@cs.sun.ac.za)
 * @date    2023-07-06
 */

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "boolean.h"

/* --- error return codes --------------------------------------------------- */

#define HASH_TABLE_KEY_VALUE_PAIR_EXISTS -1
#define HASH_TABLE_NO_SPACE_FOR_NODE     -2

/** the container structure for a hash table */
typedef struct hashtab HashTab;

/* --- function prototypes -------------------------------------------------- */

/**
 * Initialise a hash table.  This function fails if the specialised load factor
 * is less than or equal to zero, or memory could not be allocated.
 *
 * @param[in]  loadfactor
 *     the maximum load factor, which, when reached, triggers a resize of the
 *     underlying table
 * @param[in]  hash
 *     a pointer to a hash function over the domain of the keys, taking a
 *     pointer to the key and the size of the underlying table as parameters
 * @param[in]  cmp
 *     a pointer to a function that compares two values from the domain of
 *     values, returning <code>-1</code>, <code>0</code>, or <code>1</code> if
 *     <code>val1</code> is less than, equal to, or greater than
 *     <code>val2</code>, respectively
 * @return
 *     a pointer to the hash table container structure, or <code>NULL</code> if
 *     initialisation failed
 */
HashTab *ht_init(float loadfactor,
				 unsigned int (*hash)(void *key, unsigned int size),
				 int (*cmp)(void *val1, void *val2));

/**
 * Associate the specified key with the specified value in the specified hash
 * table.  This function fails if any argument is <code>NULL</code>, or if
 * insertion was not successful.
 *
 * @param[in]  ht
 *     a pointer to the hash table in which to associate the key with the value
 * @param[in]  key
 *     a pointer to the key
 * @param[in]  value
 *     a pointer to the value
 * @return
 *     <code>EXIT_SUCCESS</code> if the insertion was successful,
 *     <code>EXIT_FAILURE</code> if any argument value is <code>NULL</code>, or
 *     one of designated error codes if insertion failed
 */
int ht_insert(HashTab *ht, void *key, void *value);

/**
 * Search the specified hash table for the value associated with the specified
 * key.  This function fails if any argument is <code>NULL</code>.
 *
 * @param[in]  ht
 *     a pointer to the hash table in which to search for the key
 * @param[in]  key
 *     the key for which to find the associated value
 * @return
 *     a pointer to the value, or <code>NULL</code> if the key was not found or
 *     any argument value is <code>NULL</code>
 */
void *ht_search(HashTab *ht, void *key);

/**
 * Free the space associated with the specified hash table.  This function fails
 * if any argument is <code>NULL</code>.
 *
 * @param[in]  hashtable
 *     the hash table to free
 * @param[in]  freekey
 *     a pointer to a function that releases the memory resources of a key
 * @param[in]  freeval
 *     a pointer to a function that releases the memory resources of a value
 * @return
 *     <code>EXIT_SUCCESS</code> if the memory resources of the specified hash
 *     table were released successfully, or <code>EXIT_FAILURE</code> otherwise
 */
int ht_free(HashTab *ht,
			void (*freekey)(void *k),
			void (*freeval)(void *v));

/**
 * Display the specified hash table on standard output.  This function fails
 * silently if any argument is <code>NULL</code>.
 *
 * @param[in]  ht
 *     the hash table to display
 * @param[in]  keyval2str
 *     a pointer to a function that returns a string representation of the
 *     specified key and value in the specified buffer; the buffer is assumed to
 *     have been allocated by the caller
 */
void ht_print(HashTab *ht, void (*keyval2str)(void *k, void*v, char *b));

#endif /* HASH_TABLE_H */
