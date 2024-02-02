#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orax.h"

struct MachineInstruction {
  MachineOpcode opcode;         
  MachineRegister dest_register; 
  char *label;                   
  size_t line_number;            
};

struct InstructionTile {
  tileid_t id;
  Instruction *left, *right;
};

struct MaxMunchState {
  InstructionTile **tiles;
  size_t num_tiles;
  MachineInstruction **gen_inst; 
  size_t num_gen_inst;
};

MachineInstruction *create_machine_instruction(MachineOpcode opcode,
                                               MachineRegister dest_register,
                                               char *label,
                                               size_t line_number) {
  MachineInstruction *minst =
      (MachineInstruction *)calloc(1, sizeof(MachineInstruction));
  minst->opcode = opcode;
  minst->dest_register = dest_register;
  minst->label = label;
  minst->line_number = line_number;
  return minst;
}

InstructionTile *create_instruction_tile(tileid_t id) {
  InstructionTile *tile = (InstructionTile *)calloc(1, sizeof(InstructionTile));
  tile->id = id;
  tile->left = NULL;
  tile->right = NULL;
  return tile;
}

MaxMunchState *create_max_munch_state(void) {
  MaxMunchState *state = (MaxMunchState *)calloc(1, sizeof(MaxMunchState));
  state->tiles = NULL;
  state->gen_inst = NULL;
  return state;
}

InstructionTile *add_tile_left_subtree(InstructionTile *tile,
                                       InstructionTile *subtree) {
  tile->left = subtree;
  return tile;
}

InstructionTile *add_tile_right_subtree(InstructionTile *tile,
                                        InstructionTile *subtree) {
  tile->right = subtree;
  return tile;
}

MaxMunchState *add_munch_state_tile(MaxMunchState *state,
                                    InstructionTile *tile) {
  state->tiles = (InstructionTile **)realloc(
      state->tiles, (state->num_tiles + 1) * sizeof(InstructionTile *));
  state->tiles[state->num_tiles++] = tile;
  return state;
}

MaxMunchState *add_munch_state_inst(MaxMunchState *state,
                                    MachineInstruction *minst) {
  state->gen_inst = (MachineInstruction **)realloc(
      state->gen_inst,
      (state->num_gen_inst + 1) * sizeof(MachineInstruction *));
  state->gen_inst[state->num_gen_inst++] = minst;
  return state;
}

void free_instruction_tile(InstructionTile *tile) {
  if (tile == NULL)
    return;
  free_instruction_tile(tile->left);
  free_instruction_tile(tile->right);
  FREE_AND_NULLIFY(&tile);
}

void free_machine_register(MachineRegister *reg) {
  FREE_AND_NULLIFY(&reg->label);
}

void free_maximal_munch_state(MaxMunchState *state) {
  if (state == NULL)
    return;

  while (--state->num_tiles)
    free_instruction_tile(state->times[state->num_tiles]);

  while (--state->num_gen_inst)
    free_machine_instruction(state->gen_inst[state->num_gen_inst]);
}
