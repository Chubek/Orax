#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


#include "orax-decl.h"
#include "orax-enums.h"


struct InstructionPair {
   Instruction *inst1;
   Instruction *inst2;
   Instruction *result;
};



InstructionPair *create_inst_pair(Instruction *inst1, Instruction *inst2) {
  InstructionPair *instpair = (InstructionPair*)calloc(1, sizeof(Instruction));
  instpair->inst1 = inst1;
  instpair->inst2 = inst2;
  instpair->result = NULL;
  return instpair;
}




void fold_instpair_constants(InstructionPair *instpair) {
  
}




