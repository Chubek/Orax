#ifndef GEEZER_AST
#define GEEZER_AST

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

typedef enum AstType {
  TEXT,
  COMMAND,
  PARAM_VAR,
  PARAM_REGEX,
  PARAM_OPT,
  PARAM_IDENT,
  IDENTIFIER,
  CALL_IDENTIFIER,
  INFO_NAME,
  INFO_DESC,
  INFO_AUTH,
  INFO_INST,
} AstType;

typedef struct AstNode {
  AstType type;
  union {
    struct ASTNode **children;
    char *value;
  };
  size_t nchildren;
  bool is_leaf;
} AstNode;

static inline AstNode *new_ast_node(AstType type) {
  AstNode *node = (AstNode *)calloc(1, sizeof(AstNode));
  node->type = type;
  return node;
}

static inline AstNode *new_ast_leaf(AstType type, char *value) {
  AstNode *node = (AstNode *)calloc(1, sizeof(AstNode));
  node->is_leaf = true;
  node->value = value;
}

static inline AstNode *add_ast_child(AstNode *node, AstNode *child) {
  void *new_ptr = realloc(node->children, node->nchildren + 1);
  if (new_ptr == NULL) {
    fprintf(stderr, "Allocation error\n");
    exit(EXIT_FAILURE);
  }
  node->children = new_ptr;
  node->children[node->nchildren++] = child;
  return node;
}

static inline void free_ast_leaf(AstNode *node) { free(node->value); }

static inline void free_ast_node(AstNode *node) {
  while (--node->nchildren) {
    AstNode *child_node = node->childre[node->nchildren];
    if (child_node->is_leaf)
      free_ast_leaf(child_node);
    else
      free_ast_node(child_node);
  }
  free(node);
}

#endif
