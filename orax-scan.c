#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "orax-decl.h"
#include "orax-enums.h"


struct NFAState {
   nfaid_t id;
   bool is_accepting;
   uint16_t transitions[MAX_TRANSITIONS];
   NFAState **epsilon_transitions;
   size_t num_epsilon_transitions;
};

struct DFAState {
  dfaid_t id;
  bool is_accepting;
  uint16_t transitions[MAX_TRANSITIONS];
};

NFAState *create_nfa_state(nfaid_t id, bool is_accepting) {
   NFAState *state = (NFAState*)calloc(1, sizeof(NFAState));
   state->id = id;
   state->is_accepting = is_accepting;
   state->epislon_transitions = NULL;
   return state;
}

DFAState *create_dfa_state(dfaid_t id, bool is_accepting) {
   DFAState *state = (DFAState*)calloc(1, sizeof(DFAState));
   return state;
}

NFAState *add_nfa_epstrans(NFAState *state, NFAState *eps) {
   state->epsilon_transitions =
	(NFAState**)realloc(state->epsilon_transitions,
			(eps->num_epislon_transitions + 1) * sizeof(NFAState*));
   state->epsilon_transitions[state->num_epsilon_transitions++] = eps;
   return state;
}

void add_nfa_trans(NFAState *state, uint16_t from, uint16_t to) {
   state->transitions[from] = to;
}

void add_dfa_trans(DFAState *state, uint16_t from, uint16_t to) {
   state->transitions[from] = to;
}

DFAState *convert_nfa_to_dfa(NFAState *nfa) {
 	// implement
}
