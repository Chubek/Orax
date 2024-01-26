#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>


#include "orax-decl.h"



struct BasicBlock
{
  blockid_t id;
  BasicBlock **successors;
  size_t num_successors;
  Instruction **instructions;
  size_t num_instructions;
};



BasicBlock *create_basic_block(blockid_t block_id)
{
   BasicBlock *block = (BasicBlock*)calloc(1, sizeof(BasicBlock));
   block->id = block_id;
   block->successors = NULL;
   block->num_successors = 0;
   block->instructions = NULL;
   block->num_instructions = 0;
   return block;
}


BasicBlock *add_cfg_successor(BasicBlock *block, BasicBlock *succ)
{
   block->successors = 
	(BasicBlock**)realloc(block->successors, (block->num_successors + 1) * sizeof(BasicBlock*));
   block->successors[block->num_successors++] = succ;
   return block;
}


BasicBlock *add_cfg_instruction(BasicBlock *block, Instruction *inst)
{
  block->instructions = 
	  (Instruction**)realloc(block->instructions,
			  (block->num_instructions + 1) * sizeof(Instruction*));
  block->instructions[block->num_instructions++] = inst;
  return block;
}

BasicBlock *get_successor_by_id(BasicBlock *block, blockid_t succ_bid)
{
   for (size_t i = 0; i < block->num_successors; i++)
    if (block->successors[i]->id == succ_bid)
      return block->successors[i];

   return NULL:
}

Instruction *get_instruction_by_id(BasicBlock *block, instid_t instid)
{
  for (size_t i = 0; i < block->num_instructions; i++)
   if (block->instructions[i]->id == instid)
     return block->instructions[i];

  return NULL;
}
