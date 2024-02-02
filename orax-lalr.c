#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orax-decl.h"
#include "orax-enums.h"

#define DOT_INDEX_INIT -1

struct LR0Symbol {
  LR0SymbolType type;
  char *value;
};

struct Production {
  LR0Symbol **lr0_symbols;
  size_t num_lr0_symbols;
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

struct LR0ItemSet {
  LR0Item **items;
  size_t num_items;
};

struct LR0Closure {
  LR0Item *kernel_item;
  LR0Item **items;
  size_t num_items;
};

struct LR0GotoTableEntry {
  LR0Symbol *symbol;
  LR0ItemSet *set;
};

struct LR0GotoTable {
  LR0GotoTableEntry **entries;
  size_t num_entries;
};

struct LR0ActionTableEntry {
  LR0Symbol *symbol;
  LR0ActionType type;
  actionvalue_t value;
};

struct LR0ActionTable {
  LR0ActionTableEntry **entries;
  size_t num_entries;
};

LR0Symbol *create_lr0_symbol(LR0SymbolType type, char *value) {
  LR0Symbol *semitem = (LR0Symbol *)calloc(1, sizeof(LR0Symbol));
  semitem->type = type;
  semitem->value = value;
  return semitem;
}

LR0Symbol *create_terminal(char *value) {
  return create_lr0_symbol(LR0SYM_TERMINAL, value);
}

LR0Symbol *create_nonterminal(char *value) {
  return create_lr0_symbol(LR0SYM_NONTERMINAL, value);
}

Production *create_production(void) {
  Production *prod = (Production *)calloc(1, sizeof(Production));
  prod->lr0_symbols = NULL;
  prod->num_lr0_symbols = 0;
  return prod;
}

Production *add_production_semitem(Production *prod, LR0Symbol *semitem) {
  prod->lr0_symbols = (LR0Symbol **)realloc(
      prod->lr0_symbols, (prod->num_lr0_symbols + 1) * sizeof(LR0Symbol *));

  prod->lr0_symbols[prod->num_lr0_symbols++] = semitem;
  return prod;
}

LALRRule *create_lalr_rule(char *name, Production *production,
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

LR0ItemSet *create_lr0_set(void) {
  LR0ItemSet *set = (LR0ItemSet *)calloc(1, sizeof(LR0ItemSet));
  set->items = NULL;
  set->num_items = 0;
  return set;
}

LR0Closure *create_lr0_closure(LR0Item *kernel_item) {
  LR0Closure *closure = (LR0Closure *)calloc(1, sizeof(LR0Closure));
  closure->kernel_item = kernel_item;
  closure->items = NULL;

  lr0_closure_add_item(closure, kernel_item);
  lr0_closure_expand(closure);

  return closure;
}

LR0GotoTableEntry *create_lr0_goto_table_entry(LR0Symbol *symbol,
                                               LR0ItemSet *set) {
  LR0GotoTableEntry *entry =
      (LR0GotoTableEntry *)calloc(1, sizeof(LR0GotoTableEntry));
  entry->symbol = symbol;
  entry->set = set;
}

LR0GotoTable *create_lr0_goto_table(void) {
  LR0GotoTable *goto_table = (LR0GotoTable *)calloc(1, sizeof(LR0GotoTable));
  goto_table->entries = NULL;
  goto_table->num_entries = 0;
  return goto_table;
}

LR0ActionTableEntry *create_lr0_action_table_entry(LR0Symbol *symbol,
                                                   LR0ActionType type,
                                                   actionvalue_t value) {
  LR0ActionTableEntry *entry =
      (LR0ActionTableEntry *)calloc(1, sizeof(LR0ActionTableEntry));
  entry->symbol = symbol;
  entry->type = type;
  entry->value = value;
  return entry;
}

LR0ActionTable *create_lr0_action_table(void) {
  LR0ActionTable *action_table =
      (LR0ActionTable *)calloc(1, sizeof(LR0ActionTable));
  action_table->entries = NULL;
  action_table->num_entries = 0;
  return action_table;
}

LR0Closure *lr0_closure_add_item(LR0Closure *closure, LR0Item *item) {
  closure->items = (LR0Item **)realloc(
      closure->items, (closure->num_items + 1) * sizeof(LR0Item *));
  closure->items[closure->num_items++] = item;
  return closure;
}

LR0ItemSet *lr0_set_add_item(LR0ItemSet *set, LR0Item *item) {
  set->items =
      (LR0Item **)realloc(set->items, (set->num_items + 1) * sizeof(LR0Item *));
  set->items[set->num_items++] = item;
  return set;
}

void lr0_closure_expand(LR0Closure *closure) {
  size_t i;
  for (i = 0; i < closure->num_items; i++) {
    LR0Item *item = closure->items[i];
    if (item->dot_index < 0 ||
        item->dot_index >= item->rule->production->num_semantic_items) {
      continue;
    }

    SemanticItem *next_symbol =
        item->rule->production->semantic_items[item->dot_index];

    if (next_symbol->type == LR0SYM_NONTERMINAL) {
      LALRRule *next_rule = find_rule_by_name(next_symbol->value);
      if (next_rule != NULL) {
        LR0Item *next_item = create_lr0_item(next_rule);
        lr0_closure_add_item(closure, next_item);
      }
    }
  }
}
