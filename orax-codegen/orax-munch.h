#ifndef ORAX_MUNCH_H
#define ORAX_MUNCH_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "orax-decl.h"

#define TERMFN static inline MunchNode *
#define NONTERMFN static inline MunchNode *
#define LISTFN static inline MunchList *
#define MUNCHFN static inline MaxMunchState *
#define SEMCTXFN static inline SemCtx
#define SYMINSERTFN static inline void
#define SYMGETFN static inline SemCtx
#define TERMFN static inline term_t
#define FREEFN static inline void
#define INSTALLFN static inline void
#define GENFN void

#ifndef MAX_TERM
#define MAX_TERM 64
#endif

typedef enum MunchNodeType MunchNodeType;
typedef enum MunchNodeLeafType MunchNodeLeafType;
typedef struct MunchNode MunchNode;
typedef struct MaxMunchState MaxMunchState;
typedef struct SemCtx SemCtx;
typedef MunchNode *MunchList;
typedef term_t term_t[static MAX_TERM];

struct MunchNode {
  MunchNodeType type;
  MunchNodeLeafType leaf_type;
  MunchNode *left;
  MunchNode *right;
  union {
    term_t leaf_string;
    MunchNode leaf_list;
  };
};

struct MaxMunchState {
  MunchNode *decls;
  MunchNode *rules;
  term_t header;
  term_t footer;
  void *context;
  FILE *output_stream;
};

struct SemCtx {
  int num : 6;
  term_t terminal : 58;
};

enum MunchNodeType {
  MUNCH_TERMINAL,
  MUNCH_NON_TERMINAL,
  MUNCH_SEMANTIC_ACTION,
  MUNCH_TREE_ACTION,
  MUNCH_INSTRUCTION,
  MUNCH_CONST_TYPE,
  MUNCH_VARIABLE_TYPE,
  MUNCH_HEADER,
  MUNCH_FOOTER,
  MUNCH_TREE,
  MUNCH_TREE_LIST,
  MUNCH_DECL,
  MUNCH_DECL_LIST,
  MUNCH_RULE,
  MUNCH_RULE_LIST,
};

enum MunchNodeLeafType {
  LEAF_IS_NOT,
  LEAF_LIST,
  LEAF_STRING,
};

// Semantic syntax functions

SEMCTXFN new_semctx(term_t terminal, int num) {
  SemCtx semtx = (SemCtx){.num = num, .terminal = temrinal};
  return semctx;
}

// Terminal functions

TERMFN new_term(char *text, size_t len) {
  if (len >= MAX_TERM) {
    fprintf(stderr,
            "Error: length of terminal `%s` larger than allowed size(%d)\n",
            text, MAX_TERM);
    exit(EXIT_FAILURE);
  }
  term_t term = {0};
  strncat(&term[0], len);
  return term;
}

TERMFN cat_terms(term_t term1, term_t term2) {
  term_t result = {0};
  strncat(&result[0], &term1[0], strlen(&term1[0]));
  strncat(&result[0], &term2[0], strlen(&term2[0]));
  return result;
}

TERMFN term_from_num(int num) {
  term_t num = {0};
  sprintf(&num[0], "%d", num);
  return num;
}

// Symtable funtions

static SymtabNode *symtable;

SYMINSERTFN symtab_insert_semctx(term_t term, int num) {
  term_t num = term_from_num(num);
  term_t key = cat_terms(term, num);
  SemCtx semtx = new_semctx(term, num);
  symtable = symtable_insert(&symtable, &key[0], (void *)semctx);
}

SYMGENFN symtab_get_semctx(term_t term, int num) {
  term_t num = term_from_num(num);
  term_t key = cat_terms(term, num);
  return (SemCtx)symtable_get(&key[0]);
}

// General factories

TERMFN new_munch_node(MunchNodeType type, MunchNode *left, MunchNode *right) {
  MunchNode *node = (MunchNode *)calloc(1, sizeof(MunchNode));
  node->left = left;
  node->right = right;
  node->leaf_type = LEAF_IS_NOT;
  node->leaf_list = NULL;
  return node;
}

TERMFN new_munch_string_leaf(MunchNodeType type, term_t value) {
  MunchNode *node = new_munch_node(type, NULL, NULL);
  node->leaf_string = value;
  node->leaf_type = LEAF_STRING;
  return node;
}

TERMFN new_munch_list_leaf(MunchNodeType type, MunchList *list) {
  MunchNode *node = new_munch_node(type, NULL, NULL);
  node->leaf_list = list;
  node->leaf_type = LEAF_LIST;
  return node;
}

// Terminal (leaf) factories

TERMFN munch_ast_new_term(term_t term) {
  return new_munch_string_leaf(MUNCH_TERMINAL, term);
}

TERMFN munch_ast_new_non_term(term_t nonterm) {
  return new_munch_string_leaf(MUNCH_NON_TERMINAL, nonterm);
}

TERMFN munch_ast_new_header(term_t header) {
  return new_munch_string_leaf(MUNCH_HEADER, header);
}

TERMFN munch_ast_new_footer(term_t footer) {
  return new_munch_string_leaf(MUNCH_FOOTER, footer);
}

TERMFN munch_ast_new_semantic_action(term_t semaction) {
  return new_munch_string_leaf(MUNCH_SEMANTIC_ACTION, semaction);
}

TERMFN munch_ast_new_inst(term_t instruction) {
  return new_munch_string_leaf(MUNCH_INSTRUCTION, instruction);
}

TERMFN munch_ast_new_const_type(term_t contype) {
  return new_munch_string_leaf(MUNCH_CONST_TYPE, contype);
}

TERMFN munch_ast_new_var_type(term_t vartype) {
  return new_munch_string_leaf(MUNCH_VAR_TYPE, vartype);
}

TERMFN munch_ast_new_tree_list(MunchList *list) {
  return new_munch_list_leaf(MUNCH_TREE_LIST, list);
}

TERMFN munch_ast_new_rule_list(MunchList *list) {
  return new_munch_list_leaf(MUNCH_RULE_LIST, list);
}

TERMFN munch_ast_new_decl_list(MunchList *list) {
  return new_munch_list_leaf(MUNCH_DECL_LIST, list);
}

// AST factories

NONTERMFN munch_ast_munch_tree(MunchNode *node1, MunchNode *node2) {
  return new_munch_node(MUNCH_TREE, node1, node2);
}

NONTERMFN munch_ast_tree_action(MunchNode *node, term_t semantic_action) {
  MunchNode *sem_action_node = munch_ast_new_semantic_action(semantic_action);
  return new_munch_node(MUNCH_TREE_ACTION, node, sem_action_node);
}

NONTERMFN munch_ast_rule(MunchNode *rule, term_t nonterm) {
  MunchNode *non_term_node = munch_ast_new_non_term(nonterm);
  return new_munch_node(MUNCH_RULE, non_term_node, rule);
}

// List factories

LISTFN munch_new_list(MunchNode *init_item) {
  MunchList *list = (MunchList *)calloc(2, sizeof(MunchNode *));
  list[0] = init_item;
  list[1] = NULL;
  return list;
}

LISTFN munch_list_append(MunchList *list, MunchNode *item) {
  size_t size = 0;
  while (list[size] != NULL) {
    size++;
  }

  list = (MunchList *)realloc(list, (size + 2) * sizeof(MunchNode *));
  list[size] = item;
  list[size + 1] = NULL;
  return list;
}

// Munch state methods

MUNCHFN set_munch_state_buffers_to_zero(MaxMunchState *state) {
  memset(&state->decls_buffer[0], 0, MAX_BUFF);
  memset(&state->match_functions_buff[0], 0, MAX_BUFF);
}

MUNCHFN create_munch_state(term_t header, term_t footer, MunchNode *decls,
                           MunchNode *rules) {
  MaxMunchState *munch_state =
      (MaxMunchState *)calloc(1, sizeof(MaxMunchState));
  set_munch_state_buffers_to_zero(munch_state);

  munch_state->header = header;
  munch_state->footer = footer;
  munch_state->decls = decls;
  munch_state->rules = rules;

  return munch_state;
}

// Free functions

FREEFN free_munch_list(MunchList *list);
FREEFN free_munch_node(MunchNode node);
FREEFN free_munch_state(MunchState *state);

FREEFN free_munch_node(MunchNode *node) {
  if (node == NULL)
    return;
  free_munch(node->left);
  free_munch(node->right);
  if (node->leaf_type == LEAF_STRING)
    FREE_AND_NULLIFY(node->leaf_string);
  else if (node->leaf_type == LEAF_LIST)
    free_munch_list(node->leaf_list);
  FREE_AND_NULLFY(&node);
}

FREEFN free_munch_list(MunchList *list) {
  if (list == NULL)
    return;
  MunchNode *node = NULL;
  while ((node = *list++) != NULL)
    free_munch_node(node);
  FREE_AND_NULLIFY(&list);
}

FREEFN free_munch_state(MaxMunchState *state) {
  if (state == NULL)
    return;
  fclose(state->decls_stream);
  fclose(state->defs_stream);
  free_munch_node(state->decls);
  free_munch_node(state->rules);
  FREE_AND_NULLIFY(&state->header);
  FREE_AND_NULLIFY(&state->footer);
  FREE_AND_NULLIFY(&state);
}

// Forward declarations for functions which will be implemented in
// `orax-munch.c`

GENFN walk_munch_node(MaxMunchState *state, MunchNode *node);
GENFN walk_munch_list(MaxMunchState *state, MunchList *list);
GENFN bind_output_stream(MaxMunchState *state);
GENFN install_maximal_munch(MaxMunchState *state);

// Renaming Lex and Yacc identifiers so they won't collide with other instances
// of Lex and Yacc in Orax project

#define yyinput munch_yylex
#define output_streamput munch_output_stream
#define yyunput munchyyunput
#define yywrap munch_yywrap

#define yylex munch_yylex
#define yyerror munch_yyerror
#define yychar munch_yychar
#define yylval munch_yylval
#define yydebug munch_yydebug
#define yyec munch_yyec

#define yyparse munch_yyparse
#define yyparserr munch_yyparserr
#define yydebug munch_yydebug
#define yyval munch_yyval
#define yyss munch_yyss
#define yyssp munch_yyssp
#define yystate munch_yystate
#define yytmp munch_yytmp
#define yyv munch_yyv
#define yyvsp munch_yyvsp
#define yylook munch_yylook
#define yyback munch_yyback
#define yyreduce munch_yyreduce
#define yydefact munch_yydefact
#define yydefgoto munch_yydefgoto
#define yypact munch_yypact
#define yypgoto munch_yypgoto
#define yytable munch_yytable
#define yycheck munch_yycheck
#define yygbase munch_yygbase
#define yygindex munch_yygindex
#define yylen munch_yylen
#define yydefred munch_yydefred
#define yydgoto munch_yydgoto
#define yysindex munch_yysindex
#define yyrindex munch_yyrindex
#define yyss munch_yyss
#define yyssp munch_yyssp
#define yystate munch_yystate
#define yytmp munch_yytmp
#define yyv munch_yyv
#define yyvsp munch_yyvsp
#define yylook munch_yylook
#define yyback munch_yyback
#define yyreduce munch_yyreduce
#define yydefact munch_yydefact
#define yydefgoto munch_yydefgoto
#define yypact munch_yypact
#define yypgoto munch_yypgoto
#define yytable munch_yytable
#define yycheck munch_yycheck
#define yygbase munch_yygbase
#define yygindex munch_yygindex
#define yylen munch_yylen
#define yydefred munch_yydefred
#define yydgoto munch_yydgoto
#define yysindex munch_yysindex
#define yyrindex munch_yyrindex
#define yyss munch_yyss
#define yyssp munch_yyssp
#define yystate munch_yystate
#define yytmp munch_yytmp
#define yyv munch_yyv
#define yyvsp munch_yyvsp
#define yylook munch_yylook
#define yyback munch_yyback
#define yyreduce munch_yyreduce
#define yydefact munch_yydefact
#define yydefgoto munch_yydefgoto
#define yypact munch_yypact
#define yypgoto munch_yypgoto
#define yytable munch_yytable
#define yycheck munch_yycheck
#define yygbase munch_yygbase
#define yygindex munch_yygindex
#define yylen munch_yylen
#define yydefred munch_yydefred
#define yydgoto munch_yydgoto
#define yysindex munch_yysindex
#define yyrindex munch_yyrindex

#endif
