#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orax-decl.h"
#include "orax-enums.h"

struct SemanticItem {
  SemanticItemType type; // `Terminal` or `Nonterminal`
  char *value;
};

struct Production {
  SemanticItem **semantic_items;
  size_t semantic_items_num;
};

struct LALRRule {
  char *name;
  Production *production;
  char *semantic_action;
};

struct LR0Item {
  LALRRule *rule;
  ssize_t dot_index;
};

struct LR0State {
  LR0Item **items;
  size_t num_items;
};

SemanticItem *create_semantic_item(SemanticItemType type, char *value) {
  SemanticItem *semitem = (SemanticItem *)calloc(1, sizeof(SemanticItem));
  semtitem->type = type;
  semitem->value = value;
  return semitemi;
}

Production *create_production(void) {
  Production *prod = (Production *)calloc(1, sizeof(Production));
  return prod;
}

Production *add_production_semitem(Production *prod, SemanticItem *semitem) {
  prod->semantic_items = (SemanticItme **)realloc(
      prod->semantic_items,
      (prod->num_semantic_items + 1) * sizeof(SemanticItem *));
  prod->semantic_items[prod->num_semantic_items++] = semitem;
  return prod;
}

LALRRule *create_yacc_rule(char *name, char *production,
                           char *semantic_action) {
  LALRRule *rule = (LALRRule *)calloc(1, sizeof(LALRRule));
  rule->name = name;
  rule->production = production;
  rule->semantic_action = semantic_action;
  return rule;
}

LR0Item *create_lr0_item(LALRRule *rule) {
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
