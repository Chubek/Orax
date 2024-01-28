#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orax-decl.h"


struct ASTNode {
  ASTNodeType type;      
  ASTNode *left, *right; 
  ASTLeaf *leaf;         
};


struct ASTLeaf {
  ASTLeafType type; 
  void *value;      
};



ASTNode *create_ast_node(ASTType type) {
  ASTNode *node = (ASTNode *)calloc(1, sizeof(ASTNode));
  node->type = type;
  node->left = NULL;
  node->right = NULL;
  return node;
}



ASTLeaf *create_ast_leaf(ASTType type, void *value) {
  ASTLeaf *leaf = (ASTLeaf *)calloc(1, sizeof(ASTLeaf));
  leaf->type = type;
  leaf->value = value;
  return leaf;
}



ASTNode *add_ast_left_subtree(ASTNode *node, ASTNode *subtree) {
  node->left = subtree;
  return node;
}



ASTNode *add_ast_right_subtree(ASTNode *node, ASTNode *subtree) {
  node->right = subtree;
  return node;
}



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



void free_ast_leaf(ASTLeaf *leaf) {
  if (leaf == NULL)
    return;

  free(leaf->value);
}
