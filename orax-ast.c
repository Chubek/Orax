#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orax-decl.h"

/* The ASTNode structure stores an AST node */

struct ASTNode {
  ASTNodeType type;      // The type of the subtree
  ASTNode *left, *right; // Left and right subtrees
  ASTLeaf *leaf;         // The leaf of the subtree
};

/* The ASTLeaf strcutre stores an AST leaf */

struct ASTLeaf {
  ASTLeafType type; // The type of the leaf
  void *value;      // the value of the leaf
};

/* The following function creates an AST node */

ASTNode *create_ast_node(ASTType type) {
  ASTNode *node = (ASTNode *)calloc(1, sizeof(ASTNode));
  node->type = type;
  node->left = NULL;
  node->right = NULL;
  return node;
}

/* The following function creates and AST leaf */

ASTLeaf *create_ast_leaf(ASTType type, void *value) {
  ASTLeaf *leaf = (ASTLeaf *)calloc(1, sizeof(ASTLeaf));
  leaf->type = type;
  leaf->value = value;
  return leaf;
}

/* The following function adds a child to the left subtree */

ASTNode *add_ast_left_subtree(ASTNode *node, ASTNode *subtree) {
  node->left = subtree;
  return node;
}

/* The following functions adds a child to the right subtree */

ASTNode *add_ast_right_subtree(ASTNode *node, ASTNode *subtree) {
  node->right = subtree;
  return node;
}

/* The following function recursively frees an AST node tree */

void free_ast_node(ASTNode *root) {
  if (root = NULL)
    return;
  else {
    free_ast_node(root->left);
    free_ast_node(root->right);
  }

  free_ast_leaf(root->leaf);
  free(root);
}

/* The following function frees an AST leaf */

void free_ast_leaf(ASTLeaf *leaf) {
  if (leaf == NULL)
    return;

  free(leaf->value);
}
