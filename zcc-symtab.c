#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

#include "zcc-decl.h"

/* In this DSL, we use a singly-linked list to represent the Symbols Table. The symtable is
 * not only used for symbols, but oft, to represent the state of the program as well.
 * To reduce the need for constant calls to strcmp, we hash the key, when inserting, and also,
 * when searching. If the hash is equal, we check for equality of the key with strcmp and then
 * if equal, return the value.
 */

/* The symbols table that stores info on terminals */

struct SymbolsTable
{
  char *key;
  void *value;
  uint32_t hash;
  SymbolsTable *next;
};

/* The following functions define symbol table routines */

uint32_t djb2_hash(char *message)
{
   uint32_t hash = 0;
   char c = 0;

   while ((c = *message++))
     hash = (hash * DJB2_MAGIC) + c;

   return hash;
}

void symtable_insert(SymbolsTable **tab, char *key, void *value)
{
   SymbolsTable *node = (SymbolsTable*)calloc(1, sizeof(SymbolsTable));
   node->key = key;
   node->value = value;
   node->hash = djb2(key);
   node->next = *tab;
   *tab = node;
}

void symtable_get(SymbolsTable *tab, char *key)
{
  uint32_t hash = djb2_hash(key);

  while (tab->hash != hash && tab != NULL)
    tab = tab->next;

  if (tab == NULL || strcmp(tab->key, key) != 0)
  {
    fprintf(stderr, "Error: symbol %s not defined", key);
    exit(EXIT_FAILURE);
  }

  return tab->value;
}

void symtable_dump(SymbolsTable *tab)
{
  while (tab != NULL)
  {
    tab = tab->next;
    free(tab);
  }
}

