#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orax-decl.h"
#include "orax-enums.h"

struct PEGExpression {
  PEGSymbol symbol;
  uint32_t character;
  PEGMetaExpression subexpressions[MAX_PEG_SUBEXPRESSION];
};

union PEGMetaExpression {
  PEGexpression *pegexp;
  PEGGroup *group;
};

struct PEGGroup {
  PEGMetaExpression *expressions;
  size_t num_expressions;
};

PEGExpression *create_peg_expression(PEGSymbol symbol) {
  PEGExpression *pegxp = (PEGExpression *)calloc(1, sizeof(PEGExpression));
  peg->character = PEG_CHAR_UNASSIGNED;
  peg->subexpressions = NULL;
  return pegexp;
}

PEGGroup *create_peg_group(void) {
  PEGGroup *group = (PEGGroup *)calloc(1, sizeof(PEGGroup));
  group->expressions = NULL;
  return group;
}

PEGGroup *add_peg_group_expression(PEGGroup *group, PEGExpression *pegexp) {
  group->expressions = (PEGMetaExpressions *)realloc(
      group->expressions,
      (group->num_expressions + 1) * sizeof(PEGMetaExpression));
  group->expressions[group->num_expressions++].pegexp = pegexp;
  return group;
}

PEGGroup *add_peg_group_subgroup(PEGGroup *group, PEGGroup *group) {
  group->expressions = (PEGMetaExpressions *)realloc(
      group->expressions,
      (group->num_expressions + 1) * sizeof(PEGMetaExpression));
  group->expressions[group->num_expressions++].pegexp = group;
  return group;
}

PEGExpression *set_pegexp_char(PEGexpression *pegexp, uint32_t ch) {
  pegxp->character = ch;
}

PEGexpression *set_pegexp_sequence(PEGExpression *pegexp,
                                   PEGExpression *sequence) {
  pegexp->subexpressions[PEG_SUBEXP_INDEX_SEQUENCE].pegexp = sequence;
}

PEGexpression *set_pegexp_choice(PEGExpression *pegexp, PEGExpression *choice) {
  pegexp->subexpressions[PEG_SUBEXP_INDEX_CHOICE].pegexp = choice;
}

PEGExpression *set_pegexp_group(PEGExpression *pegexp, PEGGroup *group) {
  pegexp->subexpressions[PEG_SUBEXP_INDEX_GROUP].group = group;
}

PEGGroup *parse_pegexp_string(char *pegstr) {
  if (strlen(pegstr) == 0)
    return NULL;

  PEGGroup *group = create_peg_group();
  PEGExpression *current_expression = NULL;

  for (size_t i = 0; i < strlen(pegstr); i++) {
    if (pegstr[i] == ' ') {

      continue;
    } else if (pegstr[i] == '(') {

      PEGGroup *sub_group = parse_pegexp_string(pegstr + i + 1);
      add_peg_group_subgroup(group, sub_group);
      i += sub_group->num_expressions;
    } else if (pegstr[i] == ')') {

      return group;
    } else if (pegstr[i] == '|') {

      current_expression = set_pegexp_choice(
          current_expression, create_peg_expression(SYMBOL_CHOICE));
    } else if (pegstr[i] == '*') {

      current_expression = set_pegexp_sequence(
          current_expression, create_peg_expression(SYMBOL_ZERO_OR_MORE));
    } else if (pegstr[i] == '+') {

      current_expression = set_pegexp_sequence(
          current_expression, create_peg_expression(SYMBOL_ONE_OR_MORE));
    } else if (pegstr[i] == '?') {

      current_expression = set_pegexp_sequence(
          current_expression, create_peg_expression(SYMBOL_OPTIONAL));
    } else {

      current_expression =
          set_pegexp_char(create_peg_expression(SYMBOL_CHAR), pegstr[i]);
    }

    if (current_expression != NULL) {
      add_peg_group_expression(group, current_expression);
      current_expression = NULL;
    }
  }

  return group;
}
