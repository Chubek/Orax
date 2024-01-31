#ifndef ORAX_MUNCH_H
#define ORAX_MUNCH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "orax-decl.h"

#define NODEFN static inline MunchNode*
#define LISTFN static inline MunchList
#define MUNCHFN static inline OraxMunch*
#define FREEFN static inline void
#define INSTALLFN static inline void
#define WALKFN static inline void

typedef enum MunchNodeType MunchNodeType;
typedef struct MunchNode MunchNode;
typedef struct OraxMunch OraxMunch;
typedef MunchNode** MunchList;

struct MunchNode {
   MunchNodeType type;
   MunchNode *left;
   MunchNode *right;
   const char *value;
   bool is_leaf;
};

struct OraxMunch {
  MunchList decls;
  MunchList rules;
  const char *header;
  const char *footer;
  FILE *yyout;
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

NODEFN munch_ast_munch_tree_list(MunchList trees) {
    size_t size = 0;
    while (trees[size] != NULL) {
        size++;
    }

    if (size == 1) {
        return trees[0];
    }

    MunchNode *combined_tree = new_munch_node(MUNCH_TREE_LIST, trees[0], trees[1]);

    for (size_t i = 2; i < size; i++) {
        combined_tree = new_munch_node(MUNCH_TREE_LIST, combined_tree, trees[i]);
    }

    return combined_tree;
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

// Orax munch factory

MUNCHFN create_munch_state(const char *header, 
		const char *footer,
		MunchList decls,
		MunchList rules) {
    OraxMunch *munch_state = (OraxMunch*)calloc(1, sizeof(OraxMunch));
    munch_state->header = header;
    munch_state->footer = footer;
    munch_state->decls = decls;
    munch_state->rules = rules;
    munch_state->yyout = NULL;
    return munch_state;
}

// Free functions

FREEFN free_munch(MunchNode *node) {
    if (node == NULL)
	    return;
    free_tree(node->left);
    free_tree(node->right);
    if (node->is_leaf)
	    free(node->value);
    FREE_AND_NULLFY(&node);
}

FREEFN free_munch_list(MunchList list) {
    MunchNode *node = NULL;
    while ((node = *list++) != NULL)
	    free_munch(node);
    FREE_AND_NULLIFY(&list);
}

// C code installers

INSTALLFN munch_install_macro(OraxMunch *state, 
		const char *name, 
		const char *definition) {
    fprintf(state->yyout, "\n#define %s %s\n", name, definition);
}

INSTALLFN munch_install_word(OraxMunch *state, const char *word) {
    fprintf(state->yyout, " %s ", word);
}

INSTALLFN munch_install_char(OraxMunch *state, const char character) {
    fprintf(state->yyout, " %c ", character);
}

INSTALLFN munch_install_array_literal(OraxMunch *state, 
		const char **elements,
		const char *term) {
    char *element = NULL;
    fprintf(state->yyout, "{");
    while ((element = *elements++) != NULL)
	    fprintf(state->yyout, "%s, ", element);
    fprintf(state->yyout, "%s };", term);
}

INSTALLFN munch_install_array_identifier(OraxMunch *state,
		const char *identifier,
		const int size) {
    fprintf(state->yyout, "%s[%d]", identifier, size);
}

INSTALLFN munch_install_bitfield(OraxMunch *state,
		const char *identifier,
		const struct BitFieldItem { 
			const char *name;
			int bits;
		} **fields) {
     fprintf(state->yyout, "struct %s {", identifier);
     struct BitFieldItem *current_bitfield = NULL;
     
     while ((current_bitfield = *fields++) != NULL) {
	fprintf("%s : %d;", 
			current_bitfield->name, 
			currnet_bitfield->bits);
	FREE_AND_NULLIFY(&current_bitfield);
     }

     fprintf(state->yyout, "};");
}

// Walker functions

WALKFN walk_munch_node(MunchNode *node) {
    if (node == NULL) {
        return;
    }

    if (node->is_leaf) { }

    walk_munch_node(node->left);
    walk_munch_node(node->right);
}

WALKFN walk_munch_list(MunchList list) {
    MunchNode *node = NULL;
    while ((node = *list++) != NULL) {
        walk_munch_node(node);
    }
}

#endif
