#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orax-decl.h"
#include "orax-enums.h"

Operand *add_operands(Operand *op1, Operand *op2) {
  Operand *result = duplicate_operand(op1);

  if (operand_is_integeral(op1) && operand_is_integral(op2))
    result->signed_integral = op1->signed_integral + op2->signed_integral;
  else if (operand_pair_is_unsigned_integral(op1, op2))
    result->unsigned_integral = op1->unsigned_integral + op2->unsigned_integral;
  else if (operand_pair_is_rational(op1, op2))
    result->rational = op1->rational + op2->rational;
  else if (operand_pair_is_memory_pointer(op1, op2))
    result->memory_pointer = op1->memory_pointer + op2->memory_pointer;
   else if (
   else 
	   free_operand(result);

   return result;
}

Operand *subtract_operands(Operand *op1, Operand *op2) {
  Operand *result = duplicate_operand(op1);

  if (operand_pair_is_integral(op1, op2))
    result->signed_integral = op1->signed_integral - op2->signed_integral;
  else if (operand_pair_is_unsigned_integral(op1, op2))
    result->unsigned_integral = op1->unsigned_integral - op2->unsigned_integral;
  else if (operand_pair_is_rational(op1, op2))
    result->rational = op1->rational - op2->rational;
  else if (operand_pair_is_memory_pointer(op1, op2))
    result->memory_pointer = op1->memory_pointer - op2->memory_pointer;
  else
    free_operand(result);

  return result;
}

Operand *multiply_operands(Operand *op1, Operand *op2) {
  Operand *result = duplicate_operand(op1);

  if (operand_pair_is_integral(op1, op2))
    result->signed_integral = op1->signed_integral * op2->signed_integral;
  else if (operand_pair_is_unsigned_integral(op1, op2))
    result->unsigned_integral = op1->unsigned_integral * op2->unsigned_integral;
  else if (operand_pair_is_rational(op1, op2))
    result->rational = op1->rational * op2->rational;
  else if (operand_pair_is_memory_pointer(op1, op2))
    result->memory_pointer = op1->memory_pointer * op2->memory_pointer;
  else
    free_operand(result);

  return result;
}

Operand *divide_operands(Operand *op1, Operand *op2) {
  Operand *result = duplicate_operand(op1);

  if (operand_pair_is_integral(op1, op2))
    result->signed_integral = op1->signed_integral / op2->signed_integral;
  else if (operand_pair_is_unsigned_integral(op1, op2))
    result->unsigned_integral = op1->unsigned_integral / op2->unsigned_integral;
  else if (operand_pair_is_rational(op1, op2))
    result->rational = op1->rational / op2->rational;
  else if (operand_pair_is_memory_pointer(op1, op2))
    result->memory_pointer = op1->memeory_pointer / op2->memory_pointer;
  else
    free_operand(result);

  return result;
}

Operand *modulo_operands(Operand *op1, Operand *op2) {
  Operand *result = duplicate_operand(op1);

  if (operand_pair_is_unsigned_integral(op1, op2))
    result->unsigned_integral = op1->unsigned_integral % op2->unsigned_integral;
  else
    free_operand(result);

  return result;
}

Operand *exponentiate_operands(Operand *op1, Operand *op2) {
  Operand *result = duplicate_operand(op1);

  if (operand_pair_is_integral(op1, op2))
    result->unsigned_integral =
        pow(op1->unsigned_integral, op2->unsigned_integral);
  else
    free_operand(result);

  return result;
}

Operand *bitwise_and_operands(Operand *op1, Operand *op2) {
  Operand *result = duplicate_operand(op1);

  if (operand_pair_is_unsigned_integral(op1, op2))
    result->unsigned_integral = op1->unsigned_integral & op2->unsigned_integral;
  else
    free_operand(result);

  return result;
}

Operand *bitwise_or_operands(Operand *op1, Operand *op2) {
  Operand *result = duplicate_operand(op1);

  if (operand_pair_is_unsigned_integral(op1, op2))
    result->unsigned_integral = op1->unsigned_integral | op2->unsigned_integral;
  else
    free_operand(result);

  return result;
}

Operand *bitwise_xor_operands(Operand *op1, Operand *op2) {
  Operand *result = duplicate_operand(op1);

  if (operand_pair_is_unsigned_integral(op1, op2))
    result->unsigned_integral = op1->unsigned_integral ^ op2->unsigned_integral;
  else
    free_operand(result);

  return result;
}

Operand *bitwise_shr_operands(Operand *op1, Operand *op2) {
  Operand *result = duplicate_operand(op1);

  if (operand_pair_is_unsigned_integral(op1, op2))
    result->unsigned_integral =
        op1->unsigned_integral >> op2->unsigned_integral;
  else
    free_operand(result);

  return result;
}

Operand *bitwise_shl_operands(Operand *op1, Operand *op2) {
  Operand *result = duplicate_operand(op1);

  if (operand_pair_is_unsigned_integral(op1, op2))
    result->unsigned_integral = op1->unsigned_integral
                                << op2->unsigned_integral;
  else
    free_operand(result);

  return result;
}

void attempt_constant_folding(Instruction *inst) {
  if (inst->class != INSTCLASS_ARITHMETIC || inst->class != INSTCLASS_BITWISE)
    return;

  if (inst->num_operands != 2)
    return;
}
