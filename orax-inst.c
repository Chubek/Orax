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
  opid_t id;
  OperandType type;
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

Operand *create_operand(opid_t id, OperandType type, void *value) {
  Operand *operand = (Operand *)calloc(1, sizeof(Operand));
  operand->id = id;
  operand->ptr = value;
  return operand;
}

Operand *copy_operand(Operand *op) {
  Operand *operand_copy = (Operand*)calloc(1, sizeof(Operand));
  return memmove(operand_copy, op, sizeof(Operand));
}

Result *create_result(ResultType type, void *value) {
  return (Result *)create_operand(type, value);
}
