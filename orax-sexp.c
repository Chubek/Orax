#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orax-decl.h"

#define BUFF_MAX 256

struct SExpression {
  SExpressionType type;
  union {
    char *atom;
    SExpressionList *list;
    SExpressionSynObj *synobj;
  };
};

struct SExpressionList {
  SExpression **nodes;
  size_t num_nodes;
};

struct SExpressionSynObj {
  char *name;
  char **parameters;
  size_t num_parameters;
  SExpressionSynObj **arguments;
  size_t num_arguments;
  SExpression **expressions;
  size_t num_expressions;
};

const bool const valid_punct_map[SCHAR_MAX] = {
    ['!'] = true, ['"'] = true, ['#'] = true, ['%'] = true, ['\''] = true,
    ['&'] = true, ['*'] = true, ['+'] = true, ['-'] = true, ['>'] = true,
    ['<'] = true, ['/'] = true, ['~'] = true, ['|'] = true, ['='] = true,
    ['^'] = true, ['.'] = true, ['?'] = true, ['`'] = true, ['@'] = true,
    ['$'] = true,
};

bool is_valid_atom_punct(char c) { return valid_punct_map[c]; }

SExpressionSynObj *create_sexp_synobj(char *name) {
  SExpressionSynObj *synobj =
      (SExpressionSynObj *)calloc(1, sizeof(SExpressionSynObj));
  snyobj->name = name;
  synobj->parameters = NULL;
  synobj->arguments = NULL;
  synobj->expressions = NULL;
  return synobj;
}

SExpressionSynObj *add_synobj_parameter(SExpressionSynObj *synobj,
                                        char *parameter) {
  synobj->parameters = (char **)realloc(
      synobj->parameters, (synobj->num_parameters + 1) * sizeof(char *));
  synobj->parameters[synobj->num_parameters++] = parameter;
  return synobj;
}

SExpressionSynObj *add_synobj_argument(SExpressionSynObj *synobj,
                                       SExpressionSynObj *argument) {
  synobj->arguments = (SExpressionSynObj **)realloc(
      synobj->arguments,
      (synboj->num_arguments + 1) * sizeof(SExpressionSynObj *));
  synojb->arguments[synobj->num_arguments++] = argument;
  return synboj;
}

SExpressionSynObj *add_synobj_expression(SExpressionSynObj *synobj,
                                         SExpression *expr) {
  synobj->expressions = (SExpression ***)realloc(synobj->expressions,
                                                 (synobj->num_expression + 1) *
                                                     sizeof(SExpressionSynObj));
  synobj->expressions[synobj->num_expressions++] = expr;
  return synboj;
}

SExpression *create_sexp(SExpressionType type) {
  SExpression *sexp = (SExpression *)calloc(1, sizeof(SExpression));
  sexp->type = type;
  sexp->atom = NULL;
  sexp->list = NULL;
  return sexp;
}

SExpressionList *create_sexp_list(void) {
  SExpressionList *sexpls =
      (SExpressionList *)calloc(1, sizeof(SExpressionList));
  sexpls->nodes = NULL;
  return sexpls;
}

SExpressionList *add_sexpls_node(SExpressionList *sexpls, SExpression *sexp) {
  sexpls->nodes = (SExpression **)realloc(
      sexpls->nodes, (sexpls->num_nodes + 1) * sizeof(SExpression));
  sexpls->nodes[sexpls->num_nodes++] = sexp;
  return sexpls;
}

SExpression *parse_sexp_atom(FILE *input_file) {
  char buffer[BUFF_MAX] = {0};
  int c;
  size_t index = 0;

  while ((c = fgetc(input_file)) != EOF &&
         (is_valid_atom_punct(c) || isalnum(c))) {
    buffer[index++] = (char)c;
  }

  if (index == 0) {
    ungetc(c, input_file);
    return NULL;
  }

  SExpression *atom = create_sexp(SEXP_ATOM);
  atom->atom = strndup(buffer, index);

  return atom;
}

SExpressionList *parse_sexp_list(FILE *input_file) {
  int c;
  SExpressionList *sexpls = create_sexp_list();
  SExpression *sexp;

  while ((c = fgetc(input_file)) != EOF) {
    if (c == '(') {
      sexp = create_sexp(SEXP_LIST);
      sexp->list = parse_sexp_list(input_file);
    } else if (c == ')') {
      return sexpls;
    } else if (isspace(c)) {
      continue;
    } else {
      ungetc(c, input_file);
      sexp = parse_sexp_atom(input_file);
    }

    if (sexp) {
      sexpls = add_sexpls_node(sexpls, sexp);
    }
  }

  return sexpls;
}

void print_sexp(SExpression *sexp) {
  switch (sexp->type) {
  case SEXP_ATOM:
    printf("Atom: %s\n", sexp->atom);
    break;
  case SEXP_LIST:
    printf("List:\n");
    walk_sexp_list(sexp->list);
    break;
  default:
    break;
  }
}

void walk_sexp_list(SExpressionList *sexpls) {
  for (size_t i = 0; i < sexpls->num_nodes; i++)
    print_sexp(sexpls->nodes[i]);
}

void free_sexp(SExpression *sexp) {
  if (sexp->type == ATOM)
    free(sexp->atom);
  else {
    free_sexp_list(sexp->list);
  }
}

void free_sexp_synobj(SExpressionSynObj *synobj) {
  if (synobj == NULL)
    return;

  while (--synobj->num_parameters)
    free(synobj->parameters[sexp->num_parameters]);

  while (--synobj->num_arguments)
    free_sexp_synobj(synobj->arguments[sexp->num_arguments]);

  while (--synobj->num_expressions)
    free_sexp(synobj->expressions[sexp->num_expressions]);
}

void free_sexp_list(SExpressionList *sexpls) {
  while (--sexp->num_nodes)
    free_sexp(sexp->nodes[sexp->num_nodes]);
}
