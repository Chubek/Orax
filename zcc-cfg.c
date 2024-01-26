#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>


#include "zcc-decl.h"



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

