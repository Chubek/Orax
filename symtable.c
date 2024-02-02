#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "orax-decl.h"

struct SymtabNode {
  char *key;
  void *value;
  uint32_t hash;
  SymtabNode *next;
};

uint32_t djb2_hash(char *message) {
  uint32_t hash = 0;
  char c = 0;

  while ((c = *message++))
    hash = (hash * DJB2_MAGIC) + c;

  return hash;
}

void symtable_insert(SymtabNode **tab, char *key, void *value) {
  SymtabNode *node = (SymtabNode *)calloc(1, sizeof(SymtabNode));
  node->key = key;
  node->value = value;
  node->hash = djb2(key);
  node->next = *tab;
  *tab = node;
}

void *symtable_get(SymtabNode *tab, char *key) {
  uint32_t hash = djb2_hash(key);

  while (tab->hash != hash && tab != NULL)
    tab = tab->next;

  if (tab == NULL)
    return NULL;

  if (strncmp(tab->key, key) != 0)
    return NULL;

  return tab->value;
}

void free_symtable(SymtabNode *tab) {
  while (tab != NULL) {
    tab = tab->next;
    FREE_AND_NULLIFY(&tab);
  }
}
