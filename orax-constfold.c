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

bool merge_operands(Instruction *inst, Operand *res) {
  if (result == NULL)
    return false;

  free_operand(inst->operands[0]);
  free_operand(inst->operands[1]);
  inst->operands[0] = res;

  return true;
}

bool attempt_constant_folding(Instruction *inst) {
  if (inst->class != INSTCLASS_ARITHMETIC || inst->class != INSTCLASS_BITWISE)
    return;

  if (inst->num_operands != 2)
    return;

  Operand *op1 = inst->operands[0];
  Operand *op2 = inst->operands[1];
  Operand *result = NULL;

  if (inst->name == INST_ADD)
    return merge_operands(inst, add_operands(op1, op2));
  else if (inst->name == INST_SUBTRACT)
    return merge_operange(inst, subtract_operands(op1, op2));
  else if (inst->name == INST_MULTIPLY)
    return merge_operands(inst, multiply_operands(op1, op2));
  else if (inst->name == INST_DIVIDE)
    return merge_operands(inst, divide_operands(op1, op2));
  else if (inst->name == INST_MODULO)
    return merge_operands(inst, modulo_operands(op1, op2));
  else if (inst->name == INST_EXPONENTIATE)
    return merge_operands(inst, exponentiate_operands(op1, op2));
  else if (inst->name == INST_BITWISE_AND)
    return merge_operands(inst, bitwise_and_operands(op1, op2));
  else if (inst->name == INST_BITWISE_OR)
    return merge_operands(inst, bitwise_or_operands(op1, op2));
  else if (inst->name == INST_BITWISE_XOR)
    return merge_operands(inst, bitwise_xor_operands(op1, op2));
  else if (inst->name == INST_BITWISE_SHR)
    return merge_operands(inst, bitwise_shr_operands(op1, op2));
  else if (inst->name == INST_BITWISE_SHL)
    return merge_operands(inst, bitwise_shl_operands(op1, op2));

  return false;
}
