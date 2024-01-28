#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orax-decl.h"

struct YaccRule {
  char *name;
  char *production;
  char *semantic_action;
};

struct LR0Item {
  YaccRule *rule;
  ssize_t dot_index;
};

struct LR0State {
  LR0Item **items;
  size_t num_items;
};

YaccRule *create_yacc_rule(char *name, char *production,
                           char *semantic_action) {
  YaccRule *rule = (YaccRule *)calloc(1, sizeof(YaccRule));
  rule->name = name;
  rule->production = production;
  rule->semantic_action = semantic_action;
  return rule;
}

LR0Item *create_lr0_item(YaccRule *rule) {
  LR0Item *item = (LR0Item *)calloc(1, sizeof(LR0Item));
  item->rule = rule;
  item->dot_index = DOT_INDEX_INIT;
  return item;
}

LR0State *create_lr0_state(void) {
  LR0State *state = (LR0State *)calloc(1, sizeof(LR0State));
  state->items = NULL;
  return state;
}

LR0State *lr0_state_add_item(LR0State *state, LR0Item *item) {
  state->items = (LR0Items **)realloc(
      state->items, (state->num_items * sizeof(LR0Item)) * sizeof(LR0Item *));
  state->items[state->num_items++] = item;
  return state;
}
