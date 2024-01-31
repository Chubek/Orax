#ifndef ORAX_MUNCH_H
#define ORAX_MUNCH_H

#include <stdlib.h>
#include <stdbool.h>

#define NODEFN static inline MunchNode*
#define LISTFN static inline MunchList

typedef enum MunchNodeType MunchNodeType;
typedef struct MunchNode MunchNode;
typedef MunchNode** MunchList;

struct MunchNode {
   MunchNodeType type;
   MunchNode *left;
   MunchNode *right;
   const char *value;
   bool is_leaf;
};

enum MunchNodeType {
  MUNCH_TERMINAL,
  MUNCH_NON_TERMINAL,
  MUNCH_SEMANTIC_ACTION,
  MUNCH_INSTRUCTION,
  MUNCH_HEADER,
  MUNCH_FOOTER,
  MUNCH_TREE,
  MUNCH_TREE_LIST,
  MUNCH_DECL,
  MUNCH_DECL_LIST,
  MUNCH_RULE,
  MUNCH_RULE_LIST,
};

// General factories

NODEFN new_munch_node(MunchNodeType type, MunchNode *left, MunchNode *right) {
   MunchNode *node = (MunchNode*)calloc(1, sizeof(MunchNode));
   node->left = left;
   node->right = right;
   node->is_leaf = false;
   return node;
}

NODEFN new_munch_leaf(MunchNodeType type, const char *value) {
   MunchNode *node = new_munch_node(type, NULL, NULL);
   node->is_leaf = true;
   node->value = value;
}

// Terminal (leaf) factories

NODEFN munch_ast_new_term(const char *term) {
   return new_munch_leaf(MUNCH_TERMINAL, term);
}

NODEFN munch_ast_new_non_term(const char *nonterm) {
  return new_munch_leaf(MUNCH_NON_TERMINAL, nonterm);
}

NODEFN munch_ast_new_header(const char *header) {
  return new_munch_leaf(MUNCH_HEADER, header);
}

NODEFN munch_ast_new_footer(const char *footer) {
  return new_munch_leaf(MUNCH_FOOTER, footer);
}

NODEFN munch_ast_new_semantic_action(const char *semaction) {
  return new_munch_leaf(MUNCH_SEMANTIC_ACTION, semaction);
}

NODEFN munch_ast_new_inst(const char *instruction) {
  return new_munch_leaf(MUNCH_INSTRUCTION, instruction);
}

// AST factories


NODEFN munch_ast_munch_tree(MunchNode *node1, MunchNode *node2) {
    return new_munch_node(MUNCH_TREE, node1, node2);
}

NODEFN munch_ast_tree_action(MunchNode *node, const char *semantic_action) {
    MunchNode *semActionNode = munch_ast_new_semantic_action(semantic_action);
    return new_munch_node(MUNCH_TREE_LIST, node, semActionNode);
}

NODEFN munch_ast_rule(MunchNode *rule, const char *nonterm) {
    MunchNode *nontermNode = munch_ast_new_non_term(nonterm);
    return new_munch_node(MUNCH_RULE, nontermNode, rule);
}

// List factories

LISTFN munch_new_list(MunchNode *init_item) {
    MunchList list = (MunchList)calloc(2, sizeof(MunchNode*));
    list[0] = init_item;
    list[1] = NULL;
    return list;
}

LISTFN munch_list_append(MunchList list, MunchNode *item) {
    size_t size = 0;
    while (list[size] != NULL) {
        size++;
    }

    list = (MunchList)realloc(list, (size + 2) * sizeof(MunchNode*));
    list[size] = item;
    list[size + 1] = NULL;
    return list;
}







#endif
