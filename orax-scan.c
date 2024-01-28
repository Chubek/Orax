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

NFAState *add_nfa_epstrans(NFAState *state, NFAState *eps) {
  state->epsilon_transitions = (NFAState **)realloc(
      state->epsilon_transitions,
      (eps->num_epislon_transitions + 1) * sizeof(NFAState *));
  state->epsilon_transitions[state->num_epsilon_transitions++] = eps;
  return state;
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

          if (!state_in_set(nfa_state_set, nfa_state_set_size,
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
