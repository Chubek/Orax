#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>


#include "zcc-decl.h"



struct BasicBlock
{
  blockid_t id;
  succid_t *successors;
  ssize_t num_successors;
};



BasicBlock *create_basic_block(blockid_t block_id)
{
   BasicBlock *block = (BasicBlock*)calloc(1, sizeof(BasicBlock));
   block->id = block_id;
   block->successors = NULL;
   block->num_successors = 0;
   return block;
}


BasicBlock *add_cfg_successor(BasicBlock *block, succid_t successor_id)
{
  block->successors = (succid_t*)realloc(block->successors, block->num_successors + 1);
  block->successors[block->num_successors++] = successor_id;
  return block;
}
