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
    char *string;
    SExpressionList *list;
  };
};

struct SExpressionList {
  SExpression **nodes;
  size_t num_nodes;
};

const bool const valid_punct_map[SCHAR_MAX] = {
    ['!'] = true, ['"'] = true, ['#'] = true, ['%'] = true, ['\''] = true,
    ['&'] = true, ['*'] = true, ['+'] = true, ['-'] = true, ['>'] = true,
    ['<'] = true, ['/'] = true, ['~'] = true, ['|'] = true, ['='] = true,
    ['^'] = true, ['.'] = true, ['?'] = true, ['`'] = true, ['@'] = true,
    ['$'] = true,
};

bool is_valid_atom_punct(char c) { return valid_punct_map[c]; }

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

  SExpression *atom = create_sexp(ATOM);
  atom->atom = strndup(buffer, index);

  return atom;
}

SExpressionList *parse_sexp_list(FILE *input_file) {
  int c;
  SExpressionList *sexpls = create_sexp_list();
  SExpression *sexp;

  while ((c = fgetc(input_file)) != EOF) {
    if (c == '(') {
      sexp = create_sexp(LIST);
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
  case ATOM:
    printf("Atom: %s\n", sexp->atom);
    break;
  case LIST:
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
