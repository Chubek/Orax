#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orax-decl.h"
#include "orax-enums.h"

struct TraceBlock {
  blockid_t id;
  const char *label;
  TraceBlockType type;
  TraceBlock **successors;
  size_t num_successors;
  TraceBlock **predecessors;
  size_t num_predecessors;
  TraceBlock *immediate_dominator;
  TraceBlock **dominance_frontiers;
  size_t num_dominance_frontiers;
  Instruction **instructions;
  size_t num_instructions;
  LifeSet *live_in;
  LifeSet *live_out;
  LifeSet *use_chain;
  LifeSet *def_chain;
};

struct ControlFlowGraph {
  TraceBlock **blocks;
  size_t num_blocks;
};

TraceBlock *create_trace_block(TraceBlockType type, blockid_t block_id,
                               const char *label) {
  TraceBlock *block = (TraceBlock *)calloc(1, sizeof(TraceBlock));
  block->id = block_id;
  block->type = type;
  block->label = label;
  block->successors = NULL;
  block->predecessors = NULL;
  block->instructions = NULL;
  block->immediate_dominator = NULL;
  block->live_in = create_life_set();
  block->live_out = create_life_set();
  block->use_chain = create_life_set();
  block->def_chain = create_life_set();
  return block;
}

bool trace_blocks_are_equal(TraceBlock *block1, TraceBlock *block2) {
  return memcmp(block1, block2, sizeof(*block1));
}

ControlFlowGraph *create_cfg(void) {
  ControlFlowGraph *cfg =
      (ControlFlowGraph *)calloc(1, sizeof(ControlFlowGraph));
  return cfg;
}

ControlFlowGraph *add_cfg_trace(ControlFlowGraph *cfg, TraceBlock *block) {
  cfg->blocks = (TraceBlock **)realloc(cfg->blocks, (cfg->num_blocks + 1) *
                                                        sizeof(TraceBlock *));
  cfg->blocks[cfg->num_blocks++] = block;
  return cfg;
}

TraceBlock *add_trace_successor(TraceBlock *block, TraceBlock *succ) {
  block->successors = (TraceBlock **)realloc(
      block->successors, (block->num_successors + 1) * sizeof(TraceBlock *));
  block->successors[block->num_successors++] = succ;
  return block;
}

TraceBlock *add_trace_predecessor(TraceBlock *block, TraceBlock *pred) {
  block->predecessors = (TraceBlock **)realloc(block->predecessors,
                                               (block->num_predecessors + 1) *
                                                   sizeof(TraceBlock *));
  block->predecessors[block->num_predecessors++] = pred;
  return block;
}

TraceBlcok *add_trace_dominance_frontier(TraceBlock *block,
                                         TraceBlock *frontier) {
  block->dominance_frontiers = (TraceBlock **)realloc(
      block->dominance_frontiers,
      (block->num_dominance_frontiers + 1) * sizeof(TraceBlock *));
  block->dominance_frontiers[block->num_dominance_frontiers++] = frontier;
  return block;
}

void insert_trace_instruction_at_head(TraceBlock *block, Instruction *inst) {
  block->instructions = (Instruction **)realloc(block->instructions,
                                                (block->num_instructions + 1) *
                                                    sizeof(Instruction *));

  memmove(&block->instructions[1], &block->instructions[0],
          block->num_instructions * sizeof(Instruction *));

  block->instructions[0] = inst;
  block->num_instructions++;

  return block;
}

TraceBlock *add_trace_instruction(TraceBlock *block, Instruction *inst) {
  block->instructions = (Instruction **)realloc(block->instructions,
                                                (block->num_instructions + 1) *
                                                    sizeof(Instruction *));
  block->instructions[block->num_instructions++] = inst;
  return block;
}

void analyze_liveness(ControlFlowGraph *cfg) {
  bool changed = true;

  while (changed) {
    for (size_t i = cfg->num_blocks - 1; i >= 0; i--) {
      TraceBlock *block = cfg->blocks[i];
      LifeSet *new_live_out = copy_life_set(block->live_out);

      for (size_t j = 0; j < block->num_successors; j++)
        new_live_out =
            union_life_set(new_live_out, block->successors[j]->live_in);

      LifeSet *diff = difference_life_set(new_live_out, block->def_chain);
      LifeSet *new_live_in = union_life_set(block->use_chain, diff);

      if (!objects_are_equal(new_live_in, block->live_in) ||
          !objects_are_equal(new_live_out, block->live_out)) {
        changed = true;
        free_life_set(block->live_in);
        free_life_set(block->live_out);
        block->live_in = new_live_in;
        block->live_out = new_live_out;
      } else {
        free_life_set(new_live_in);
        free_life_set(new_live_out);
      }
    }
  }
}

void ssa_calculate_immediate_dominator(ControlFlowGraph *cfg,
                                       size_t entry_index) {

  cfg->blocks[entry_index]->immediate_dominator = cfg->blocks[entry_index];

  for (size_t i = 0; i < cfg->num_blocks; i++) {
    if (i != entry_index)
      cfg->blocks[i]->immediate_dominator = NULL;
  }

  bool changed = true;
  while (changed) {
    changed = false;

    for (size_t i = 0; i < cfg->num_blocks; i++) {
      if (i == entry_index)
        continue;

      TraceBlock *block = cfg->blocks[i];

      TraceBlock *new_idom = NULL;

      for (size_t j = 0; j < block->num_predecessors; j++) {
        TraceBlock *pred = block->predecessors[j];

        if (pred->immediate_dominator != NULL) {
          if (new_idom == NULL)
            new_idom = pred->immediate_dominator;
          else {

            TraceBlock *runner1 = new_idom;
            TraceBlock *runner2 = pred->immediate_dominator;

            while (runner1 != runner2) {
              while (runner1->id > runner2->id)
                runner1 = runner1->immediate_dominator;

              while (runner1->id < runner2->id)
                runner2 = runner2->immediate_dominator;
            }

            new_idom = runner1;
          }
        }
      }

      if (!trace_blocks_are_equal(block->immediate_dominator, new_idom)) {
        block->immediate_dominator = new_idom;
        changed = true;
      }
    }
  }
}

void ssa_calculate_dominance_frontiers(ControlFlowGraph *cfg) {

  for (size_t i = 0; i < cfg->num_blocks; i++) {
    cfg->blocks[i]->dominance_frontiers = NULL;
    cfg->blocks[i]->num_dominance_frontiers = 0;
  }

  for (size_t i = cfg->num_blocks - 1; i > 0; i--) {
    TraceBlock *block = cfg->blocks[i];

    for (size_t j = 0; j < block->num_successors; j++) {
      TraceBlock *succ = block->successors[j];

      if (succ->immediate_dominator != block) {
        succ = add_trace_dominance_frontier(succ, block);
      }
    }

    for (size_t j = 0; j < block->num_dominance_frontiers; j++) {
      TraceBlock *frontier_block = block->dominance_frontiers[j];

      for (size_t k = 0; k < block->num_successors; k++) {
        TraceBlock *succ = block->successors[k];

        if (succ->immediate_dominator != block) {
          succ = add_trace_dominance_frontier(succ, frontier_block);
        }
      }
    }
  }
}

void ssa_reversion_operands(ControlFlowGraph *cfg) {
  ophash_t hash_lut[MAX_OP_HASH] = {0};

  for (size_t i = 0; i < cfg->num_blocks; i++) {
    TraceBlock *block = cfg->blocks[i];

    for (size_t j = 0; j < block->num_instructions; j++) {
      Instruction *inst = block->instructions[j];

      for (size_t k = 0; k < inst->num_operands; k++) {
        Operand *operand = inst->operands[k];

        if (operand->ssa_version == SSA_VERSION_UNASSIGNED) {
          hash_lut[operand->hash] = hash_lut[operand->hash] == 0
                                        ? GEN_UNIQUE_ID()
                                        : hash_lut[operand->hash]++;
          new_operand->ssa_version = hash_lut[operand->hash];
        }
      }
    }
  }
}

void ssa_insert_phi_instructions(ControlFlowGraph *cfg) {

  for (size_t i = 0; i < cfg->num_blocks; i++) {
    TraceBlock *block = cfg->blocks[i];

    for (size_t j = 0; j < block->num_dominance_frontiers; j++) {
      TraceBlock *dominance_frontier = block->dominance_frontiers[j];

      for (size_t k = 0; k < dominance_frontier->num_instructions; k++) {
        Instruction *inst = dominance_frontier->instructions[k];

        for (size_t m = 0; m < inst->num_operands; m++) {
          Operand *operand = inst->operands[m];

          for (size_t x = 0; x < dominance_frontier->num_predecessors; x++) {
            TraceBlock *pred = dominance_frontier->predecessors[x];

            if (dominates(pred, block)) {

              if (operand_defined_in_block(operand, pred)) {

                if (!is_phi_present_for_operand(block, operand)) {
                  Instruction *phi_inst =
                      create_phi_instruction(block, operand);
                  insert_instruction_at_head(block, phi_inst);
                }
              }
            }
          }
        }
      }
    }
  }
}

bool dominates(TraceBlock *dominator, TraceBlock *block) {

  while (block != NULL && block->immediate_dominator != dominator) {
    block = block->immediate_dominator;
  }
  return (block == dominator);
}

bool operand_defined_in_block(Operand *operand, TraceBlock *block) {
  for (size_t i = 0; i < block->num_instructions; i++) {
    Instruction *inst = block->instructions[i];
    for (size_t j = 0; j < inst->num_operands; j++) {
      if (inst->operands[j] == operand) {
        return true;
      }
    }
  }
  return false;
}

bool is_phi_present_for_operand(TraceBlock *block, Operand *operand) {
  for (size_t i = 0; i < block->num_instructions; i++) {
    Instruction *inst = block->instructions[i];
    if (inst->type == INST_PHI) {
      for (size_t j = 0; j < inst->num_operands; j++) {
        if (inst->operands[j] == operand) {
          return true;
        }
      }
    }
  }
  return false;
}

Instruction *create_phi_instruction(TraceBlock *block, Operand *operand) {
  Instruction *phi_inst = create_instruction(INST_PHI, INSTCLASS_SSA, GEN_UNIQUE_ID());
  Operand *phi_result = create_operand(operand->hash, operand->type, NULL);
  phi_result->ssa_version = GEN_UNIQUE_ID();
  phi_inst = add_inst_result(phi_inst, phi_result);

  for (size_t i = 0; i < block->num_predecessors; i++) {
    TraceBlock *pred = block->predecessors[i];
    Operand *pred_operand = get_operand_from_block(operand, pred);
    phi_inst = add_inst_operand(phi_inst, pred_operand);
  }

  return phi_inst;
}

Operand *get_operand_from_block(Operand *operand, TraceBlock *block) {
  for (size_t i = 0; i < block->num_instructions; i++) {
    Instruction *inst = block->instructions[i];
    for (size_t j = 0; j < inst->num_operands; j++) {
      if (inst->operands[j] == operand) {
        return inst->operands[j];
      }
    }
  }
  return NULL;
}

void insert_instruction_at_head(TraceBlock *block, Instruction *inst) {
  block->instructions = (Instruction **)realloc(block->instructions,
                                                (block->num_instructions + 1) *
                                                    sizeof(Instruction *));
  memmove(&block->instructions[1], &block->instructions[0],
          block->num_instructions * sizeof(Instruction *));
  block->instructions[0] = inst;
  block->num_instructions++;
}

TraceBlock *get_successor_by_id(TraceBlock *block, blockid_t succ_bid) {
  for (size_t i = 0; i < block->num_successors; i++)
    if (block->successors[i]->id == succ_bid)
      return block->successors[i];

  return NULL:
}

Instruction *get_instruction_by_id(TraceBlock *block, instid_t instid) {
  for (size_t i = 0; i < block->num_instructions; i++)
    if (block->instructions[i]->id == instid)
      return block->instructions[i];

  return NULL;
}

void free_trace_block(TraceBlock *block) {
  if (block == NULL)
    return;

  while (--block->num_successors) {
    free_trace_block(block->successors[block->num_successors]);
  }

  while (--block->num_predecessors) {
    free_trace_block(block->prececessors[block->num_predecessors]);
  }

  while (--block->num_dominance_frontiers) {
    free_trace_block(
        block->dominance_frontiers[block->num_dominance_frontiers]);
  }

  free_life_set(block->live_in);
  free_life_set(block->live_out);
  free_life_set(block->use_chain);
  free_life_set(block->def_chain);

  free_trace_block(block->immediate_dominator);
  FREE_AND_NULLIFY(&block);
}

void free_cfg(ControlFlowGraph *cfg) {
  if (cfg == NULL)
    return;
  while (--cfg->num_blocks)
    free_trace_block(cfg->blocks[cfg->num_blocks]);
  FREE_AND_NULLIFY(&cfg);
}
