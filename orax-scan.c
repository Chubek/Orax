#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orax-decl.h"
#include "orax-enums.h"

struct NFAState {
  nfaid_t id;
  bool is_accepting;
  uint32_t transitions[MAX_TRANSITIONS];
  NFAState **epsilon_transitions;
  size_t num_epsilon_transitions;
};

struct DFAState {
  dfaid_t id;
  bool is_accepting;
  uint32_t transitions[MAX_TRANSITIONS];
};

struct StackAutomaton {
  NFAState **states;
  NFAState *top_of_stack;
  size_t num_states;
};

struct LexicalRule {
  StackAutomaton *automaton;
  char *semantic_action;
};

struct LexicalStartCondition {
  condid_t id;
  char *name;
  bool exclusive;
};

struct LexicalScannerSpecs {
  LexicalStartCondition **start_conditions;
  size_t num_start_conditions;
  LexicalRule **lexical_rules;
  size_t num_lexical_rules;
};

NFAState *create_nfa_state(nfaid_t id, bool is_accepting) {
  NFAState *state = (NFAState *)calloc(1, sizeof(NFAState));
  state->id = id;
  state->is_accepting = is_accepting;
  state->epislon_transitions = NULL;
  return state;
}

DFAState *create_dfa_state(dfaid_t id, bool is_accepting) {
  DFAState *state = (DFAState *)calloc(1, sizeof(DFAState));
  return state;
}

StackAutomaton *create_stack_automaton(void) {
  StackAutomaton *automaton =
      (StackAutomaton *)calloc(1, sizeof(StackAutomaton));
  automaton->states = NULL;
  return automaton;
}

LexicalRule *create_lexical_rule(char *semantic_action) {
  LexicalRule *lrule = (LexicalRule *)calloc(1, sizeof(LexicalRule));
  lrule->automaton = NULL;
  lrule->semantic_action = strdup(semantic_action);
  return lrule;
}

LexicalStartCondition *create_lexical_start_condition(condid_t id, char *name,
                                                      bool exclusive) {
  LexicalStartCondition *stcond =
      (LexicalStartConditio *)calloc(1, sizeof(LexicalStartCondition));
  stcond->id = id;
  stcond->name = name;
  stcond->exclusive = exclusive;
  return stcond;
}

LexicalScannerSpecs *create_lexical_scanner_specs(void) {
  LexicalScannerSpecs *lspecs =
      (LexicalScannerSpecs *)calloc(1, sizeof(LexicalScannerSpecs));
  lspecs->start_conditions = NULL;
  lspecs->lexical_rules = NULL;
  return lspecs;
}

NFAState *add_nfa_epstrans(NFAState *state, NFAState *eps) {
  state->epsilon_transitions = (NFAState **)realloc(
      state->epsilon_transitions,
      (eps->num_epislon_transitions + 1) * sizeof(NFAState *));
  state->epsilon_transitions[state->num_epsilon_transitions++] = eps;
  return state;
}

StackAutomaton *push_stack_state(StackAutomaton *stack, NFAState *state) {
  stack->states = (NFAState **)realloc(stack->states, (stack->num_states + 1) *
                                                          sizeof(NFAState *));
  stack->states[stack->num_states++] = state;
  return stack;
}

LexicalScannerSpecs *add_lspecs_start_condition(LexicalScannerSpecs *lspecs,
                                                LexicalStartCondition *stcond) {
  lspecs->start_conditions = (LexicalStartCondition **)realloc(
      lspecs->start_conditions,
      (lspecs->num_start_conditions + 1) * sizeof(LexicalStartCondition *));
  lspecs->start_conditions[lspecs->num_start_conditions++] = stcond;
  return lspecs;
}

LexicalScannerSpecs *add_lspecs_lexical_rule(LexicalScannerSpecs *lspecs,
                                             LexicalRule *lrule) {
  lspecs->lexical_rules = (LexicalRule **)realloc(
      lspecs->lexical_rules,
      (lspecs->num_lexical_rules + 1) * sizeof(LexicalRule *));
  lspecs->lexical_rules[lspecs->num_lexical_rules++] = lrule;
  return lspecs;
}

NFAState *stack_tos_proceed(StackAutomaton *stack) {
  return ++stack->top_of_stack;
}
NFAState *stack_tos_recede(StackAutomaton *stack) {
  return --staci->top_of_stack;
}

void add_nfa_trans(NFAState *state, uint32_t from, uint32_t to) {
  state->transitions[from] = to;
}

void add_dfa_trans(DFAState *state, uint32_t from, uint32_t to) {
  state->transitions[from] = to;
}

DFAState *convert_nfa_to_dfa(NFAState *nfa) {
  DFAState *dfa_states[MAX_TRANSITIONS] = {NULL};
  size_t num_dfa_states = 0;

  NFAState *nfa_start_state = nfa;
  NFAState *dfa_start_state =
      create_dfa_state(num_dfa_states, nfa_start_state->is_accepting);

  NFAState *epsilon_closure[MAX_TRANSITIONS] = {NULL};
  size_t epsilon_closure_size = 0;

  epsilon_closure[epsilon_closure_size++] = nfa_start_state;

  for (size_t i = 0; i < epsilon_closure_size; ++i) {
    NFAState *current_state = epsilon_closure[i];

    for (size_t j = 0; j < current_state->num_epsilon_transitions; ++j) {
      NFAState *epsilon_transition_state =
          current_state->epsilon_transitions[j];

      if (!state_in_set(epsilon_closure, epsilon_closure_size,
                        epsilon_transition_state)) {
        epsilon_closure[epsilon_closure_size++] = epsilon_transition_state;
      }
    }
  }

  DFAState *dfa_start_state = create_dfa_state(num_dfa_states, false);

  for (size_t i = 0; i < epsilon_closure_size; ++i) {
    dfa_start_state->id |= (1 << epsilon_closure[i]->id);
    if (epsilon_closure[i]->is_accepting) {
      dfa_start_state->is_accepting = true;
    }
  }

  dfa_states[num_dfa_states++] = dfa_start_state;

  for (size_t dfa_state_index = 0; dfa_state_index < num_dfa_states;
       ++dfa_state_index) {
    DFAState *current_dfa_state = dfa_states[dfa_state_index];

    for (uint32_t input_symbol = 0; input_symbol < MAX_TRANSITIONS;
         ++input_symbol) {
      if (input_symbol == EPSILON) {
        continue;
      }

      DFAState *next_dfa_state = create_dfa_state(num_dfa_states, false);
      NFAState *nfa_state_set[MAX_TRANSITIONS] = {NULL};
      size_t nfa_state_set_size = 0;

      for (size_t i = 0; i < MAX_TRANSITIONS; ++i) {
        if (current_dfa_state->id & (1 << i)) {
          nfa_state_set[nfa_state_set_size++] = nfa->transitions[input_symbol];
        }
      }

      for (size_t i = 0; i < nfa_state_set_size; ++i) {
        NFAState *nfa_state = nfa_state_set[i];

        for (size_t j = 0; j < nfa_state->num_epsilon_transitions; ++j) {
          NFAState *epsilon_transition_state =
              nfa_state->epsilon_transitions[j];

          4 if (!state_in_set(nfa_state_set, nfa_state_set_size,
                              epsilon_transition_state)) {
            nfa_state_set[nfa_state_set_size++] = epsilon_transition_state;
          }
        }
      }

      for (size_t i = 0; i < nfa_state_set_size; ++i) {
        next_dfa_state->id |= (1 << nfa_state_set[i]->id);
        if (nfa_state_set[i]->is_accepting) {
          next_dfa_state->is_accepting = true;
        }
      }

      bool state_found = false;
      for (size_t i = 0; i < num_dfa_states; ++i) {
        if (memcmp(next_dfa_state, dfa_states[i], sizeof(DFAState)) == 0) {
          state_found = true;
          break;
        }
      }

      if (!state_found) {
        dfa_states[num_dfa_states++] = next_dfa_state;
      }

      add_dfa_trans(current_dfa_state, input_symbol, next_dfa_state->id);
    }
  }

  return dfa_start_state;
}

bool state_in_set(NFAState **set, size_t size, NFAState *state) {
  for (size_t i = 0; i < size; ++i) {
    if (set[i] == state) {
      return true;
    }
  }
  return false;
}

const bool const legal_operators[SCHAR_MAX] = {
    ['*'] = true, ['|'] = true, ['('] = true, [')'] = true, ['?'] = true,
    ['+'] = true, ['.'] = true, ['['] = true, [']'] = true,
};

const precedence_t const precedence_map[SCHAR_MAX] = {
    ['|'] = 1,
    ['*'] = 2,
    ['?'] = 2,
    ['+'] = 2,
};

const char const escape_map[SCHAR_MAX] = {
    [']'] = ']',  ['n'] = '\n', ['\\'] = '\\', ['a'] = '\a',
    ['b'] = '\b', ['"'] = '"',  ['\''] = '\'', ['r'] = '\r',
    ['t'] = '\t', ['s'] = ' ',  ['v'] = '\v',  ['?'] = '?',
    ['+'] = '+',  ['*'] = '*',  ['('] = '(',   [')'] = ')',
};

bool is_operator(char c) { return legal_operators[c]; }
precedence_t precedence(char c) { return precedence_map[c]; }

void infix_to_postfix(const char *regex, char *postfix) {
  int i = 0, j = 0;
  char stack[strlen(regex)];
  int top = -1;

  while (regex[i] != '\0') {
    char current = regex[i];

    if (!is_operator(current)) {
      postfix[j++] = current;
    } else {
      while (top >= 0 && precedence(stack[top]) >= precedence(current)) {
        postfix[j++] = stack[top--];
      }
      stack[++top] = current;
    }

    i++;
  }

  while (top >= 0) {
    postfix[j++] = stack[top--];
  }

  postfix[j] = '\0';
}

StackAutomaton *parse_regular_expression(const char *regex) {
  char postfix[strlen(regex) + 1];
  infix_to_postfix(regex, postfix);

  StackAutomaton *stack = create_stack_automaton();

  int i = 0;
  while (postfix[i] != '\0') {
    char current = postfix[i];

    if (!is_operator(current)) {
      NFAState *state = create_nfa_state(i, false);
      add_nfa_trans(state, current, i + 1);
      push_stack_state(stack, state);
    } else if (current == '|') {
      NFAState *right = stack_tos_recede(stack);
      NFAState *left = stack_tos_recede(stack);

      NFAState *state = create_nfa_state(i, false);
      add_nfa_epstrans(state, left);
      add_nfa_epstrans(state, right);
    } else if (current == '*') {
      NFAState *top = stack_tos_recede(stack);

      NFAState *state = create_nfa_state(i, false);
      add_nfa_epstrans(state, top);
      add_nfa_epstrans(top, state);

      stack = push_stack_state(stack, state);
    } else if (current == '?') {
      NFAState *top = stack_tos_recede(stack);

      NFAState *state = create_nfa_state(i, false);
      add_nfa_epstrans(state, top);

      stack = push_stack_state(stack, state);
    } else if (current == '+') {
      NFAState *top = stack_tos_recede(stack);

      NFAState *state = create_nfa_state(i, false);
      add_nfa_epstrans(state, top);
      add_nfa_epstrans(top, state);

      stack = push_stack_state(stack, state);
    } else if (current == '.') {
      NFAState *state = create_nfa_state(i, false);
      add_nfa_trans(state, ANY_CHAR, i + 1);

      push_stack_state(stack, state);
    } else if (current == '[') {
      NFAState *state = create_nfa_state(i, false);
      int j = i + 1;
      char range_start = 0, range_end = 0;

      while ((range_start = postfix[j]) != ']') {
        if (postfix[j + 1] == '-') {
          if (postfix[j + 2] != ']') {
            range_end = postfix[j + 2];
          }
        }

        if (range_start != 0 && range_end != 0 && range_start < range_end) {
          for (char r = range_start; r <= range_end; r++)
            add_nfa_trans(state, r, i + 1);
          range_start = range_end = 0;
        } else
          add_nfa_trans(state, range_start, i + 1);
        j++;
      }

      stack = push_stack_state(stack, state);
      i = j;
    } else if (current == ']') {
      fprintf(
          stderr,
          "Error: Close character class ']' without open '[' at position %d\n",
          i);
      exit(EXIT_FAILURE);
    } else if (current == '\\') {
      char next_char = postfix[++i];
      NFAState *state = create_nfa_state(i, false);
      add_nfa_trans(state, escape_map[next_char], i + 1);
      stack = push_stack_state(stack, state);
    }

    i++;
  }

  stack->top_of_stack->is_accepting = true;
  return stack;
}

void lexer_generator_output_dfa_state(DFAState *dfa, FILE *output_file) {
  fprintf(output_file,
          "const char const STATE_TRANSITIONS_%d[MAX_TRANSITIONS] = {\n",
          dfa->id);
  for (uint32_t i = 0; i < MAX_TRANSITIONS; i++) {
    if (dfa->transitions[i])
      fprintf(output_file, "[%lc] = %lc, ", (uint16_t)i,
              (uint16_t)dfa->transitions[i]);

    if ((i + 1) % NUM_GENERATED_ARRAY_ROWS == 0)
      fprintf(output_file, SYSTEM_NEWLINE);
  }
  fprintf(output_file, "\n};\n");
}

void lexer_generator_output_dfa_accepting_status(DFAState **states,
                                                 size_t num_states,
                                                 FILE *output_file) {
  fprintf(output_file, "const bool const STATE_ACCEPTANCE_MAP[%lu] = {",
          num_states);
  while (--num_states) {
    if (dfa[num_states]->is_accepting)
      fprintf(output_file, "[%d] = %s, ", dfa->id, "true");

    if ((num_states) % NUM_GENERATED_ARRAY_ROWS == 0)
      fprintf(output_file, SYSTEM_NEWLINE);
  }
  fprintf(output_file, "\n};\n");
}

void free_nfa_state(NFAState *nfa) {
  if (nfa == NULL)
    return;

  while (--nfa->num_epsilon_transitions)
    free_nfa_state(nfa->epsilon_transitions[nfa->num_epsilon_transitions]);

  FREE_AND_NULLIFY(&nfa);
}

void free_dfa_state(DFAState *dfa) {
  if (dfa == NULL)
    return;

  FREE_AND_NULLIFY(&dfa);
}

void free_stack_automaton(StackAutomaton *stack) {
  while (--stack->num_states)
    free_nfa_state(stack->states[stack->num_states]);

  FREE_AND_NULLIFY(&stack);
}

void free_lexical_rule(LexicalRule *lrule) {
  free_stack_automaton(lrule->automaton);
  FREE_AND_NULLIFY(&lrule->semantic_action);
  FREE_AND_NULLIFY(&lrule);
}
