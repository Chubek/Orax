#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orax-decl.h"

struct Instruction {
  instid_t id;
  InstructionType type;
  Operand **operands;
  size_t num_operands;
  Result *result;
};

struct Operand {
  ophash_t hash;
  ssaversion_t ssa_version;
  OperandType type;
  size_t size;
  union {
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

Instruction *create_instruction(InstructionType type, instid_t id) {
  Instruction *inst = (Instruction *)calloc(1, sizeof(Instruction));
  inst->id = id;
  inst->type = type;
  inst->operands = NULL;
  inst->num_operands = 0;
  inst->result = NULL;
  return inst;
}

Instruction *add_inst_operand(Instrution *inst, Operand *operand) {
  inst->operands = (Operand **)realloc(inst->operands, (inst->num_operand + 1) *
                                                           sizeof(Operand *));
  inst->operands[inst->num_operands++] = operand;
  return inst;
}

Instruction *add_inst_result(Instruction *inst, Result *result) {
  inst->result = result;
  return inst;
}

Operand *create_operand(ophash_t hash, OperandType type, void *value) {
  Operand *operand = (Operand *)calloc(1, sizeof(Operand));
  operand->hash = hash;
  operand->ssa_version = SSA_VERSION_UNASSIGNED;
  operand->ptr = value;
  operand->size = operand->type = type;
  return operand;
}

Operand *duplicate_operand(Operand *op) {
  Operand *operand_copy = (Operand *)calloc(1, sizeof(Operand));
  operand_copy = memmove(operand_copy, op, sizeof(Operand));
  operand_copy->id = GEN_UNIQUE_ID();
  return operand_copy;
}

Result *create_result(ResultType type, void *value) {
  return (Result *)create_operand(type, value);
}

void free_operand(Operand *op) {
  if (op == NULL)
    return;

  if (op->type == OPTYPE_PTR && op->ptr != NULL)
    FREE_AND_NULLIFY(&op->ptr);
  else if (op->type == OPTYPE_STR && op->str != NULL)
    FREE_AND_NULLIFY(&op->str);

  FREE_AND_NULLIFY(&op);
}

void free_instruction(Instruction *inst) {
  while (--inst->num_operands)
    free_operand(inst->operands[inst->num_operands]);

  free_operand(inst->result);

  FREE_AND_NULLIFY(&inst);
}
