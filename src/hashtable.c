#include "hashtable.h"

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This creates a new hash table of the specified size and with
 * the given hash function and comparison function.
 */
HashTable *createHashTable(int size, unsigned int (*hashFunction)(void *),
                           int (*equalFunction)(void *, void *)) {
  int i = 0;
  HashTable *newTable = malloc(sizeof(HashTable));
  if (NULL == newTable) {
    fprintf(stderr, "malloc failed \n");
    exit(1);
  }
  newTable->size = size;
  newTable->buckets = malloc(sizeof(struct HashBucketEntry *) * size);
  if (NULL == newTable->buckets) {
    fprintf(stderr, "malloc failed \n");
    exit(1);
  }
  for (i = 0; i < size; i++) {
    newTable->buckets[i] = NULL;
  }
  newTable->hashFunction = hashFunction;
  newTable->equalFunction = equalFunction;
  return newTable;
}

/* Task 1.2 */
void insertData(HashTable *table, void *key, void *data) {
  // -- TODO --
  // HINT:
  // 1. Find the right hash bucket location with table->hashFunction.
  int idx = table->hashFunction(key);

  // 2. Allocate a new hash bucket entry struct.
  HashBucketEntry * entry = (HashBucketEntry *)malloc(sizeof(HashBucketEntry));
  entry->key = key;
  entry->data = data;

  // 3. Append to the linked list or create it if it does not yet exist. 
  entry->next = table->buckets[idx];
  table->buckets[idx] = entry;
}

/* Task 1.3 */
void *findData(HashTable *table, void *key) {
  // -- TODO --
  // HINT:
  // 1. Find the right hash bucket with table->hashFunction.
  int idx = table->hashFunction(key);

  // 2. Walk the linked list and check for equality with table->equalFunction.
  HashBucketEntry * p = table->buckets[idx];
  while (p && !table->equalFunction(key, p->key))
    p = p->next;
  return p;
}

/* Task 2.1 */
unsigned int stringHash(void *s) {
  // -- TODO --
  /* To suppress compiler warning until you implement this function, */
  unsigned int seed = 131;
  unsigned hash = 0;
  char * p = (char *)s;
  while(*p)
  {
    hash = hash * seed + *p;
    ++p;
  }
      
  return hash;
}

/* Task 2.2 */
int stringEquals(void *s1, void *s2) {
  // -- TODO --
  /* To suppress compiler warning until you implement this function */
  char * p1 = (char *)s1;
  char * p2 = (char *)s2;

  if (p1 == NULL || p2 == NULL)
    return 0;
  while (*p1 && *p2)
  {
    if (*p1 != *p2)
      return 0;
    ++p1;
    ++p2;
  }
  return 1;
}