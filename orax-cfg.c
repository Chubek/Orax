#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>


#include "orax-decl.h"



struct TraceBlock
{
  blockid_t id;
  TraceBlock **successors;
  size_t num_successors;
  Instruction **instructions;
  size_t num_instructions;
  LifeSet *live_in;
  LifeSet *live_out;
  LifeSet *use_set;
  LifeSet *def_set;
};

struct ControlFlowGraph
{
  TraceBlock **blocks;
  size_t num_blocks;
};

TraceBlock *create_trace_block(blockid_t block_id)
{
   TraceBlock *block = (TraceBlock*)calloc(1, sizeof(TraceBlock));
   block->id = block_id;
   block->successors = NULL;
   block->num_successors = 0;
   block->instructions = NULL;
   block->num_instructions = 0;
   block->live_in = create_life_set();
   block->live_out = create_life_set();
   block->use_set = create_life_set();
   block->def_set = create_life_set();
   return block;
}

ControlFlowGraph *create_cfg(void)
{
   ControlFlowGraph *cfg = (ControlFlowGraph*)calloc(1, sizeof(ControlFlowGraph));
   return cfg;
}


ControlFlowGraph *add_cfg_trace(ControlFlowGraph *graph, TraceBlock *block)
{
  graph->blocks =
      (TraceBlock**)realloc(graph->blocks, (graph->num_blocks + 1) * sizeof(TraceBlock*));
  graph->blocks[graph->num_blocks++] = block;
  return graph;
}


TraceBlock *add_trace_successor(TraceBlock *block, TraceBlock *succ)
{
   block->successors = 
	(TraceBlock**)realloc(block->successors, (block->num_successors + 1) * sizeof(TraceBlock*));
   block->successors[block->num_successors++] = succ;
   return block;
}


TraceBlock *add_trace_instruction(TraceBlock *block, Instruction *inst)
{
  block->instructions = 
	  (Instruction**)realloc(block->instructions,
			  (block->num_instructions + 1) * sizeof(Instruction*));
  block->instructions[block->num_instructions++] = inst;
  return block;
}

void analyze_liveness(ControlFlowGraph *graph)
{
  bool changed = true;

  while (changed)
  {
     for (size_t i = graph->num_blocks - 1; i >= 0; i--)
     {
        TraceBlock *block = graph->blocks[i];
	LifeSet *new_live_out = copy_life_set(block->live_out);

	for (size_t j = 0; j < block->num_successors; j++)
	   new_live_out = union_life_set(new_live_out, block->successors[j]->live_in);

	LifeSet *diff = difference_life_set(new_live_out, block->def_set);
	LifeSet *new_live_in = union_life_set(block->use_set, diff);

	if (!objects_are_equal(new_live_in, block->live_in) 
			|| !objects_are_equal(new_live_out, block->live_out)) {
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

TraceBlock *get_successor_by_id(TraceBlock *block, blockid_t succ_bid)
{
   for (size_t i = 0; i < block->num_successors; i++)
    if (block->successors[i]->id == succ_bid)
      return block->successors[i];

   return NULL:
}


Instruction *get_instruction_by_id(TraceBlock *block, instid_t instid)
{
  for (size_t i = 0; i < block->num_instructions; i++)
   if (block->instructions[i]->id == instid)
     return block->instructions[i];

  return NULL;
}

void free_cfg(TraceBlock *block)
{
   free_life_set(block->live_in);
   free_life_set(block->live_out);
   free_life_set(block->used_set);
   free_life_set(block->def_set);
   free(block);
}



