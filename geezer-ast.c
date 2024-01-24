#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "geezer-decl.h"

/* An Abstract Syntax Tree is an assymetic tree which is usually the first method
 * of an intermediate representation after the text in the language has been parsed,
 * and there has been no lexical, or parsing errors. AST represents the syntax of the
 * program in a way that it can be considered a 'pruned' version of the parse tree,
 * hence the term 'abstract'. However, an AST does not define the relationships between
 * parts of the syntax. The AST is usually when the text is scanned for syntactic errors, 
 * and then it is converted to another form of intermediate representation.
 * In a compiled language, in modern times, that's usually the DAG and then the CFG and
 * from there, the SSA. However, Geezer is just a tiny DSL so we just translate it to a DAG.
 * See `geezer-dag.c` for mor info.
 * A tree data structure usually has a 'node' and a 'leaf'. In an AST, the leaves are used
 * to represent terminals such as commands and parameters, and nodes are used to encompass
 * them.
 */


/* The ASTNode structure stores an AST node, or leaf */

struct ASTNode
{
  ASTType type;				// The type
  ASTNode *left, *right;		// AST subtrees
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
  node->value = value;
  return node;
}

/* The following function adds a child to the tree */

ASTNode *ast_add_child(ASTNode **parent, ASTNode *child)
{
   (*parent)->left = child;
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

ASTNode *new_ast_pkg(char *id, ASTNode *body)
{
  ASTNode *id_leaf = new_ast_leaf(AST_NODE_PKGIDENT);
  ASTNode *pkg_node = new_ast_node(AST_NODE_PKGDEF);
  pkg_node->left = id_leaf;
  pkg_node->right = body;
  return pkg_node;
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
  ASTNode *id_leaf = new_ast_leaf(AST_LEAF_INSTIDENT, id);
  ASTNode *inst_node = new_ast_node(AST_NODE_INST);
  inst_node->left = id_leaf;
  inst_node->right = invoke;
  return inst_node;
}

ASTNode *new_ast_invoke(char *id, ASTNode *args)
{
  ASTNode *id_leaf = new_ast_leaf(AST_LEAF_INVOKEIDENT, id);
  ASTNode *invoke_node = new_ast_node(AST_NODE_INVOKE);
  invoke_node->left = id_leaf;
  invoke_node->right = args;
  return invoke_node;
}

ASTNode *new_ast_param(char *value)
{
  return new_ast_leaf(AST_LEAF_PARAM, value);
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
