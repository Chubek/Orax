#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orax-decl.h"

struct Instruction {
  instid_t id;
  InstructionClass class;
  InstructionName type;
  Operand **operands;
  size_t num_operands;
  Result *result;
};

struct Operand {
  ophash_t hash;
  ssaversion_t ssa_version;
  OperandType type;
  bool literal;
  size_t size;
  union {
    unsigned_integral_t unsigned_integeral;
    signed_integral_t signed_integral;
    rational_t rational;
    memory_pointer_t memory_pointer;
    boolean_t boolean;
  };
};

Instruction *create_instruction(InstructionName type, InstructionClass class, instid_t id) {
  Instruction *inst = (Instruction *)calloc(1, sizeof(Instruction));
  inst->id = id;
  inst->class = class;
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

Operand *create_operand(ophash_t hash, OperandType type, void *value, size_t size) {
  Operand *operand = (Operand *)calloc(1, sizeof(Operand));
  operand->hash = hash;
  operand->ssa_version = SSA_VERSION_UNASSIGNED;
  operand->size = size; 
  operand->type = type;

  switch (type) {
	case OPTYPE_UNSIGNED_INTEGRAL:
		operand->unsigned_integeral = *((unsigned_integral_t*)value);
		break;
	case OPTYPE_SIGNED_INTEGRAL:
		operand->signed_integral = *((signed_integral_t*)value);
		break;
	case OPTYPE_RATIONAL:
		operand->rational = *((rational_t*)value);
		break;
	case OPTYPE_MEMORY_POINTER:
		operand->memory_pointer = value;
		break;
	case OPTYPE_BOOLEAN:
		operand->boolean = *((boolean_t*)value);
		break;
	default:
		break;
  }

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
