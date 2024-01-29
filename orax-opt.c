#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orax-decl.h"
#include "orax-enums.h"

void attempt_constant_folding(Instruction *inst) {
  if (inst->class != INSTCLASS_ARITHMETIC)
    return;
  if (inst->num_operands < 2)
    return;

   Operand *result = create_operand(
}
