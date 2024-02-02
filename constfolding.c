#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orax.h"
#include "enumerations.h"

Variable *add_variables(Variable *op1, Variable *op2) {
  Variable *result = duplicate_variable(op1);

  if (variable_is_integeral(op1) && variable_is_integral(op2))
    result->signed_integral = op1->signed_integral + op2->signed_integral;
  else if (variable_pair_is_unsigned_integral(op1, op2))
    result->unsigned_integral = op1->unsigned_integral + op2->unsigned_integral;
  else if (variable_pair_is_rational(op1, op2))
    result->rational = op1->rational + op2->rational;
  else if (variable_pair_is_memory_pointer(op1, op2))
    result->memory_pointer = op1->memory_pointer + op2->memory_pointer;
  else
    free_variable(result);

  return result;
}

Variable *subtract_variables(Variable *op1, Variable *op2) {
  Variable *result = duplicate_variable(op1);

  if (variable_pair_is_integral(op1, op2))
    result->signed_integral = op1->signed_integral - op2->signed_integral;
  else if (variable_pair_is_unsigned_integral(op1, op2))
    result->unsigned_integral = op1->unsigned_integral - op2->unsigned_integral;
  else if (variable_pair_is_rational(op1, op2))
    result->rational = op1->rational - op2->rational;
  else if (variable_pair_is_memory_pointer(op1, op2))
    result->memory_pointer = op1->memory_pointer - op2->memory_pointer;
  else
    free_variable(result);

  return result;
}

Variable *multiply_variables(Variable *op1, Variable *op2) {
  Variable *result = duplicate_variable(op1);

  if (variable_pair_is_integral(op1, op2))
    result->signed_integral = op1->signed_integral * op2->signed_integral;
  else if (variable_pair_is_unsigned_integral(op1, op2))
    result->unsigned_integral = op1->unsigned_integral * op2->unsigned_integral;
  else if (variable_pair_is_rational(op1, op2))
    result->rational = op1->rational * op2->rational;
  else if (variable_pair_is_memory_pointer(op1, op2))
    result->memory_pointer = op1->memory_pointer * op2->memory_pointer;
  else
    free_variable(result);

  return result;
}

Variable *divide_variables(Variable *op1, Variable *op2) {
  Variable *result = duplicate_variable(op1);

  if (variable_pair_is_integral(op1, op2))
    result->signed_integral = op1->signed_integral / op2->signed_integral;
  else if (variable_pair_is_unsigned_integral(op1, op2))
    result->unsigned_integral = op1->unsigned_integral / op2->unsigned_integral;
  else if (variable_pair_is_rational(op1, op2))
    result->rational = op1->rational / op2->rational;
  else if (variable_pair_is_memory_pointer(op1, op2))
    result->memory_pointer = op1->memeory_pointer / op2->memory_pointer;
  else
    free_variable(result);

  return result;
}

Variable *modulo_variables(Variable *op1, Variable *op2) {
  Variable *result = duplicate_variable(op1);

  if (variable_pair_is_unsigned_integral(op1, op2))
    result->unsigned_integral = op1->unsigned_integral % op2->unsigned_integral;
  else
    free_variable(result);

  return result;
}

Variable *exponentiate_variables(Variable *op1, Variable *op2) {
  Variable *result = duplicate_variable(op1);

  if (variable_pair_is_integral(op1, op2))
    result->unsigned_integral =
        pow(op1->unsigned_integral, op2->unsigned_integral);
  else
    free_variable(result);

  return result;
}

Variable *bitwise_and_variables(Variable *op1, Variable *op2) {
  Variable *result = duplicate_variable(op1);

  if (variable_pair_is_unsigned_integral(op1, op2))
    result->unsigned_integral = op1->unsigned_integral & op2->unsigned_integral;
  else
    free_variable(result);

  return result;
}

Variable *bitwise_or_variables(Variable *op1, Variable *op2) {
  Variable *result = duplicate_variable(op1);

  if (variable_pair_is_unsigned_integral(op1, op2))
    result->unsigned_integral = op1->unsigned_integral | op2->unsigned_integral;
  else
    free_variable(result);

  return result;
}

Variable *bitwise_xor_variables(Variable *op1, Variable *op2) {
  Variable *result = duplicate_variable(op1);

  if (variable_pair_is_unsigned_integral(op1, op2))
    result->unsigned_integral = op1->unsigned_integral ^ op2->unsigned_integral;
  else
    free_variable(result);

  return result;
}

Variable *bitwise_shr_variables(Variable *op1, Variable *op2) {
  Variable *result = duplicate_variable(op1);

  if (variable_pair_is_unsigned_integral(op1, op2))
    result->unsigned_integral =
        op1->unsigned_integral >> op2->unsigned_integral;
  else
    free_variable(result);

  return result;
}

Variable *bitwise_shl_variables(Variable *op1, Variable *op2) {
  Variable *result = duplicate_variable(op1);

  if (variable_pair_is_unsigned_integral(op1, op2))
    result->unsigned_integral = op1->unsigned_integral
                                << op2->unsigned_integral;
  else
    free_variable(result);

  return result;
}

bool merge_variables(Instruction *inst, Variable *res) {
  if (result == NULL)
    return false;

  free_variable(inst->variables[0]);
  free_variable(inst->variables[1]);
  inst->variables[0] = res;

  return true;
}

bool attempt_constant_folding(Instruction *inst) {
  if (inst->class != INSTCLASS_ARITHMETIC || inst->class != INSTCLASS_BITWISE)
    return;

  if (inst->num_variables != 2)
    return;

  Variable *op1 = inst->variables[0];
  Variable *op2 = inst->variables[1];
  Variable *result = NULL;

  if (inst->name == INST_ADD)
    return merge_variables(inst, add_variables(op1, op2));
  else if (inst->name == INST_SUBTRACT)
    return merge_operange(inst, subtract_variables(op1, op2));
  else if (inst->name == INST_MULTIPLY)
    return merge_variables(inst, multiply_variables(op1, op2));
  else if (inst->name == INST_DIVIDE)
    return merge_variables(inst, divide_variables(op1, op2));
  else if (inst->name == INST_MODULO)
    return merge_variables(inst, modulo_variables(op1, op2));
  else if (inst->name == INST_EXPONENTIATE)
    return merge_variables(inst, exponentiate_variables(op1, op2));
  else if (inst->name == INST_BITWISE_AND)
    return merge_variables(inst, bitwise_and_variables(op1, op2));
  else if (inst->name == INST_BITWISE_OR)
    return merge_variables(inst, bitwise_or_variables(op1, op2));
  else if (inst->name == INST_BITWISE_XOR)
    return merge_variables(inst, bitwise_xor_variables(op1, op2));
  else if (inst->name == INST_BITWISE_SHR)
    return merge_variables(inst, bitwise_shr_variables(op1, op2));
  else if (inst->name == INST_BITWISE_SHL)
    return merge_variables(inst, bitwise_shl_variables(op1, op2));

  return false;
}
