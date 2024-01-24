#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

#include "geezer.h"

/* This macro is used to recurse the tree nodes, and leaves */

#define RECURSE_TREE_NODE()									\
  walk_and_translate(root->right->right == NULL ? root->right->left : root->right->left,	\
                          callbackfn);								\
  walk_and_translate(root->left->right == NULL ? root->left->left : root->left->right,		\
                          callbackfn)


#define RECURSE_TREE_LEAF()									\
  walk_and_translate(root->left)


extern FILE *yyout;	// from `geezer-lexer.l`


/* `geezer-sdt.c` is responsible for syntx-directed translation of a Geezer DSL text into 
 * the target languages. It achieves so via a tree-walker function that acts off of 
 * callback functions given to it.
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

/* This function is the general AST walker, accepts a callback function based on the target language */

void walk_and_translate(ASTNode *root, SdtCallbackFn callbackfn)
{ 
    if (root == NULL || root->type == AST_LEAF_TERMINATE)
      return;

    if (!root->is_leaf)
    {
       if (root->type == AST_NODE_CMDDEF)
       {
  	  callbackfn(FLAG_SET_MODE | FLAG_MODE_CMD, NULL);
	  ASTNode *ident = node->left;
	  ASTNode *params = node->right;
	  ASTNode *command = node->right->left;
	  walk_and_translate(ident, callbackfn);
	  walk_and_translate(params, callbackfn);
	  walk_and_translate(command, callbackfn);
	  callbackfn(FLAG_UNSET_MODE | FLAG_MODE_CMD, NULL);
	  node->right->left = NULL;	// We have to set this node as NULL to recurse properly
	  RECURSE_TREE_NODE();
       }

      if (root->type == AST_NODE_PKGDEF)
      {
    	 callbackfn(FLAG_SET_MODE | FLAG_MODE_PKG, NULL);
	 ASTNode *ident = root->left;
	 ASTNode *body = root->right;
	 walk_and_translate(ident, callbackfn);
	 walk_and_translate(body, callbackfn);
	 callbackfn(FLAG_UNSET_MODE | FLAG_MODE_PKG, NULL);
	 RECURSE_TREE_NODE();
      }

      if (root->type == AST_NODE_INSTDEF)
      {
	 callbackfn(FLAG_SET_MODE | FLAG_MODE_INST, NULL);
	 ASTNode *ident = root->left;
	 ASTNode *invoke = root->right;
	 walk_and_translate(ident, callbackfn);
	 walk_and_translate(invoke, callbackfn);
	 callbackfn(FLAG_UNSET_MODE | FLAG_MODE_INST, NULL);
	 RECURSE_TREE_NODE();
      }
   }

    switch (root->type)
    {
       case AST_LEAF_CMDIDENT:
	callbackfn(FLAG_MODE_CMD | FLAG_IDENT, root->value);
	break;
       case AST_LEAF_ELLIPSE:
	callbackfn(FLAG_MODE_CMD | FLAG_PARAM | FLAG_VARARG, NULL);
	break;
       case AST_LEAF_SHOWRD:
	callbackfn(FLAG_MODE_INST | FLAG_SHELL_WORD, root->value);
	break;
       case AST_LEAF_REFIDENT:
	callbackfn(FLAG_MODE_CMD | FLAG_REFERENCE | FLAG_IDENT, root->value);
       case AST_LEAF_INFOIDENT:
	callbackfn(FLAG_MODE_PKG | FLAG_IDENT


    }
}
