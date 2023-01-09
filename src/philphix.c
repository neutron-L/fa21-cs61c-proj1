/*
 * Include the provided hash table library.
 */
#include "hashtable.h"

/*
 * Include the header file.
 */
#include "philphix.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>
#include <assert.h>
/*
 * This hash table stores the dictionary.
 */
HashTable *dictionary;

/*
 * The MAIN routine.  You can safely print debugging information
 * to standard error (stderr) as shown and it will be ignored in 
 * the grading process.
 */
#ifndef _PHILPHIX_UNITTEST
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Specify a dictionary\n");
    return 1;
  }
  /*
   * Allocate a hash table to store the dictionary.
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(0x61C, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");

  fprintf(stderr, "Processing stdin\n");
  processInput();

  /*
   * The MAIN function in C should always return 0 as a way of telling
   * whatever program invoked this that everything went OK.
   */
  return 0;
}
#endif /* _PHILPHIX_UNITTEST */

/* Task 3 */
void readDictionary(char *dictName) {
  // -- TODO --
  static unsigned int DEFAULT_LEN = 60;
  FILE * fp = fopen(dictName, "r");

  // if file does not exist 
  if (fp == NULL)
  {
    fprintf(stderr, "fail to open file %s\n", dictName);
    exit(61);
  }

  int ch;
  char * key, *value;
  char * buf = (char *)malloc(DEFAULT_LEN * sizeof(char));
  unsigned int len = DEFAULT_LEN;
  unsigned int idx = 0;
  int is_key = 1;
  while (1)
  {
    ch = fgetc(fp); 
    // extend space
    if (idx == len)
    {
      len += DEFAULT_LEN;
      buf = (char *)realloc(buf, len * sizeof(char));
    }   
    
    if (isspace(ch) || ch == EOF)
    {
      if (idx != 0)
      {
        char *tmp;
        buf[idx++] = '\0';
        tmp = (char *)malloc(idx * sizeof(char));
        strcpy(tmp, buf);
        if (is_key)
          key = tmp;
        else
          value = tmp;
        if (!is_key)
        {
           // insert key and data
          insertData(dictionary, key, value); 
          // fprintf(stderr, "insert key %s value %s\n", key, value);
        }
        is_key ^= 1;
        idx = 0;
      }

      if (ch == EOF)
        break;
    }
    else
    {
      if (is_key && !isalnum(ch))
      {
        fprintf(stderr, "invalid key character %c\n", ch);
        exit(1);
      }
      buf[idx++] = ch;
    }
  }

  free(buf); // release buffer space
  fclose(fp);
}

/* Task 4 */
void processInput() {
  // -- TODO --
  static unsigned int DEFAULT_LEN = 60;

  int ch;
  unsigned int len = DEFAULT_LEN;
  char *word = (char *)malloc(len * sizeof(char));
  unsigned int idx = 0;
  char * value = NULL;
  while (1)
  { 
    ch = getchar();
    
    if (!isalnum(ch) || ch == EOF)
    {
      // has extracted a word, try to translate
      if (idx != 0)
      {
        word[idx] = '\0';
        value = (char *)findData(dictionary, word);
        // the exact word
        if (value)
          printf("%s", value);
        else
        {
          // backup word
          // char * tmp = strdup(word);
          char * tmp = (char *)malloc((idx + 1) * sizeof(char));
          tmp[0] = word[0];
          // The word with every alphabetical character converted to uppercase 
          for (unsigned int i = 1; i < idx + 1; ++i)
            tmp[i] = tolower(word[i]);
          tmp[idx] = '\0';
          value = (char *)findData(dictionary, tmp);
          if (value)
            printf("%s", value);
          else
          {
            // The exact word, with the number 1 prepended before it
            tmp[0] = tolower(tmp[0]);
            value = (char *)findData(dictionary, tmp);
            if (value)
              printf("%s", value);
            else
              printf("%s", word);
          }
          free(tmp);
        }

        idx = 0;
      }
      if (ch == EOF)
        break;
      printf("%c", ch);
      continue;
    }

    // expand space
    if (idx == len)
    {
      len += DEFAULT_LEN;
      word = (char *)realloc(word, len * sizeof(char));
    }
    word[idx++] = ch;
  }

  free(word);
}
