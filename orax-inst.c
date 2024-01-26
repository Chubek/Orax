#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "orax-decl.h"


struct Instruction
{
  instid_t id;
  InstructionType type;
  Operand **operands_defined;
  Operand **opernds_used;
  size_t num_operands_defined;
  size_t num_operands_used;
  Result *result;
};

struct Operand
{
  OperandType type;
  union
  {
    uint64_t u64;
    int64_t i64;
    uint32_t u32;
    int32_t i32;
    uint16_t u16;
    int64_t i16;
    uint8_t u8;
    int8_t i8;
    float f32;
    double f64;
    long double f80;
    void *ptr;
    char *str;
  };
};

Instruction *create_instruction(InstructionType type, instid_t id)
{
  Instruction *inst = (Instruction*)calloc(1, sizeof(Instruction));
  inst->id = id;
  inst->type = type;
  inst->operands = NULL;
  inst->num_operands = 0;
  inst->result = NULL;
  return inst;
}

Instruction *add_inst_operand_defined(Instrution *inst, Operand *operand)
{
  inst->operands_defined = 
     (Operand**)realloc(inst->operands_defined, (inst->num_operand_defined + 1) * sizeof(Operand*));
  inst->operands_defined[inst->num_operands_defined++] = operand;
  return inst;
}

Instruction *add_inst_operand_used(Instrution *inst, Operand *operand)   
{
  inst->operands_used = 
     (Operand**)realloc(inst->operands_used, (inst->num_operand_used + 1) * sizeof(Operand*));
  inst->operands_used[inst->num_operands_used++] = operand;
  return inst;
}

Instruction *add_inst_result(Instruction *inst, Result *result)
{
  inst->result = result;
  return inst;
}

Operand *create_operand(OperandType type, void *value)
{
   Operand *operand = (Operand*)calloc(1, sizeof(Operand));
   operand->ptr = value;
   return operand;
}

Result *create_result(ResultType type, void *value)
{
  return (Result*)create_operand(type, value);
}






