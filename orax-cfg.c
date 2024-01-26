#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orax-decl.h"

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
  LifeSet *use_set;
  LifeSet *def_set;
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
  block->use_set = create_life_set();
  block->def_set = create_life_set();
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

      LifeSet *diff = difference_life_set(new_live_out, block->def_set);
      LifeSet *new_live_in = union_life_set(block->use_set, diff);

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

void calculate_immediate_dominator(ControlFlowGraph *cfg, size_t entry_index) {

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

void calculate_dominance_frontiers(ControlFlowGraph *cfg) {

  for (size_t i = 0; i < cfg->num_blocks; i++) {
    cfg->blocks[i]->dominance_frontiers = NULL;
    cfg->blocks[i]->num_dominance_frontiers = 0;
  }

  for (size_t i = cfg->num_blocks - 1; i > 0; i--) {
    TraceBlock *block = cfg->blocks[i];

    for (size_t j = 0; j < block->num_successors; j++) {
      TraceBlock *succ = block->successors[j];

      if (succ->immediate_dominator != block) {

        size_t num_frontiers = succ->num_dominance_frontiers;
        succ->dominance_frontiers =
            (TraceBlock **)realloc(succ->dominance_frontiers,
                                   (num_frontiers + 1) * sizeof(TraceBlock *));
        succ->dominance_frontiers[num_frontiers] = block;
        succ->num_dominance_frontiers++;
      }
    }

    for (size_t j = 0; j < block->num_dominance_frontiers; j++) {
      TraceBlock *frontier_block = block->dominance_frontiers[j];

      for (size_t k = 0; k < block->num_successors; k++) {
        TraceBlock *succ = block->successors[k];

        if (succ->immediate_dominator != block) {

          size_t num_frontiers = succ->num_dominance_frontiers;
          succ->dominance_frontiers = (TraceBlock **)realloc(
              succ->dominance_frontiers,
              (num_frontiers + 1) * sizeof(TraceBlock *));
          succ->dominance_frontiers[num_frontiers] = frontier_block;
          succ->num_dominance_frontiers++;
        }
      }
    }
  }
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

void free_cfg(TraceBlock *block) {
  free_life_set(block->live_in);
  free_life_set(block->live_out);
  free_life_set(block->used_set);
  free_life_set(block->def_set);
  free(block);
}
