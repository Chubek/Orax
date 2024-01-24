#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "geezer.h"

/* The following data structues define Abstract Syntax Tree for the Geezer langauge.
 * Geezer is a language used to store data about packages, libraries, and it provides
 * ways to define commands to install them.
 * When the `geezer(1)` command is invoked, the Flex lexer scans the passed document
 * and passes it to the Yacc parser which in turn translates the document into the following AST.
 * This AST, then, is used to construct an executable binary which can be invoked to print data
 * about libraries and packages, or install them
 */


/* The ASTNode structure stores an AST node, or leaf */

struct ASTNode
{
  ASTType type;				// The type
  struct ASTNode *left, right;		// AST subtrees
  bool is_leaf;				// Is this a leaf? Leaves represent terminals
  char *value;				// This only will be non-NULL if leaf
};


/* The following function creates an AST node */

ASTNode *new_ast_node(ASTType type)
{
   ASTNode *node = (ASTNode*)calloc(1, sizeof(ASTNode));
   node->type = type;
   node->left = NULL;
   node->right = NULL;
   return node;
}

/* The following function creates an AST leaf */

ASTNode *new_ast_leaf(ASTType type, char *value)
{
  ASTNode *node = new_ast_node(type);
  node->is_leaf = true;
  node->value = valeu;
  return node;
}

/* The following function adds a child to the tree */

ASTNode *ast_add_child(ASTNode **parent, ASTNode *child)
{
   (*parent)->child = child;
   return *parent;
}

/* The following functions are factories for AST nodes and leaves */

ASTNode *new_ast_info(char *id, char *text)
{
   ASTNode *id_leaf = new_ast_leaf(AST_LEAF_INFOIDENT, id);
   ASTNode *txt_leaf = new_ast_leaf(AST_LEAF_INFOTXT, text);
   ASTNode *info_node = new_ast_node(AST_NODE_INFO);
   info_node->left = id_leaf;
   info_node->right = txt_leaf;
   return info_node;
}

ASTNode *new_ast_cmd(char *id, ASTNode *params, ASTNode *command)
{
  ASTNode *id_leaf = new_ast_leaf(AST_LEAF_CMDIDENT, id);
  ASTNode *cmd_node = new_ast_node(AST_NODE_CMDDEF);
  cmd_node->left = id_leaf;
  cmd_node->right = params;
  cmd_node->right->left = command;
  return cmd_node;
}

ASTNode *new_ast_inst(char *id, ASTNode *invoke)
{
  ASTNode *id_leaf = new_ast_leaf(AST_LEAF_INSTIDENT);
  ASTNode *inst_node = new_ast_node(AST_NODE_INST);
  inst_node->left = id_leaf;
  inst_node->right = invoke;
  return inst_node;
}

ASTNode *new_ast_cmdident(char *value)
{
  return new_ast_leaf(AST_LEAF_CMDIDENT, value);
}


ASTNode *new_ast_ellipse(void)
{
  return new_ast_leaf(AST_LEAF_ELLIPSES, NULL);
}

ASTNode *new_ast_shword(char *value)
{
  return new_ast_leaf(AST_LEAF_SHWORD, value);
}


ASTNode *new_ast_cmdtxt(char *value)
{
  return new_ast_leaf(AST_LEAF_CMDTXT, value);
}

ASTNode *new_ast_refident(char *value)
{
  return new_ast_leaf(AST_LEAF_REFIDENT, value);
}

ASTNode *new_ast_terminate(void)
{
  return new_ast_leaf(AST_LEAF_TERMINATE, NULL);
}

void free_ast(ASTNode *root)
{
  if (root->is_leaf && root->value != NULL)
    free(root->value);
  if (root->left != NULL)
    free(root->left);
  if (root->right != NULL)
    free(root->right);
  if (root != NULL)
    free(root);
}
