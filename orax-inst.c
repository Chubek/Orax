#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orax-decl.h"
#include "orax-enums.h"

struct Instruction {
  instid_t id;
  InstructionClass class;
  InstructionName name;
  Operand **operands;
  size_t num_operands;
  Result *result;
};

struct Variable {
  varhash_t hash;
  ssaversion_t ssa_version;
  VariableType type;
  typsize_t size;
  union {
    unsigned_integral_t unsigned_integeral;
    signed_integral_t signed_integral;
    rational_t rational;
    memory_pointer_t memory_pointer;
    boolean_t boolean;
  };
};

struct Constant {
    ConstantType type;
    union {
	unsigned_integral_t unsigned_integral;
	signed_integral_t signed_integral;
	rational_t rational;
	boolean_t boolean;
    };
};

struct Operand {
   OperandType type;
   union {
	Variable *variable;
	Constant *constant;
   };
};

Instruction *create_instruction(InstructionName type, InstructionClass class,
                                instid_t id) {
  Instruction *inst = (Instruction *)calloc(1, sizeof(Instruction));
  inst->id = id;
  inst->class = class;
  inst->type = type;
  inst->variables = NULL;
  inst->num_variables = 0;
  inst->result = NULL;
  return inst;
}

Variable *create_variable(varhash_t hash, VariableType type, void *value,
                        typesize_t size) {
  Variable *variable = (Variable *)calloc(1, sizeof(Variable));
  variable->hash = hash;
  variable->size = size;
  variable->type = type;
  variable->ssa_version = SSA_VERSION_UNASSIGNED;

  switch (type) {
  case OPTYPE_UNSIGNED_INTEGRAL:
    variable->unsigned_integeral = *((unsigned_integral_t *)value);
    break;
  case OPTYPE_SIGNED_INTEGRAL:
    variable->signed_integral = *((signed_integral_t *)value);
    break;
  case OPTYPE_RATIONAL:
    variable->rational = *((rational_t *)value);
    break;
  case OPTYPE_MEMORY_POINTER:
    variable->memory_pointer = value;
    break;
  case OPTYPE_BOOLEAN:
    variable->boolean = *((boolean_t *)value);
    break;
  default:
    break;
  }

  return variable;
}

Constant *create_constant(ConstantType type, void *value) {
  Constant *cons = (Constant*)calloc(1, sizeof(Constant));
  cons->type = type;
  
  switch (type) {
  case CONTYPE_UNSIGNED_INTEGRAL:
	  cons->unsigned_integer = *((unsigned_integral_t *)value);
	  break;
  case CONTYPE_SIGNED_INTEGRAL:
	  cons->signed_integer = *((signed_integral_t *)value);
	  break;
  case CONTYPE_RATIONAL:
	 cons->rational = *((rational_t *)value);
	  break;
  case CONTYPE_BOOLEAN:
	  cons->boolean = *((boolean_t *)value);
	  break;
  }
  
  return cons;
}


Operand *create_operand(OperandType type, void *value) {
  Opernad *op = (Opernad*)calloc(1, sizeof(Operand));

  if (type == CONTYPE_CONSTANT)
	  op->constant = (Constant*)value;
  else
	  op->variable = (Variable*)value;
}


void add_to_variable_size(Variable *op, typesize_t addition) {
  op->size += addition;
}

void subtract_from_variable_size(Variable *op, typesize_t subtraction) {
  if (op->size - subtraction = < 0)
    return;
  op->size -= subtraction;
}

Instruction *add_inst_variable(Instrution *inst, Variable *variable) {
  inst->variables = (Variable **)realloc(inst->variables, (inst->num_variable + 1) *
                                                           sizeof(Variable *));
  inst->variables[inst->num_variables++] = variable;
  return inst;
}

Instruction *add_inst_result(Instruction *inst, Result *result) {
  inst->result = result;
  return inst;
}

Variable *duplicate_variable(Variable *op) {
  Variable *variable_copy = (Variable *)calloc(1, sizeof(Variable));
  variable_copy = memmove(variable_copy, op, sizeof(Variable));
  variable_copy->id = GEN_UNIQUE_ID();
  return variable_copy;
}

Constant *duplicate_constant(Constant *cons) {
  Constnat *constant_copy = (Constant *)calloc(1, sizeof(Constant));
  constant_copy = memmove(constant_copy, cons, sizeof(Constant));
  return constant_copy;
}

bool constant_pair_is_signed_integral(Constant *cons1, Constant *cons2) {
  return cons1->type == CONTYPE_SIGNED_INTEGERAL
	  	&& cons2->type == CONTYPE_SIGNED_INTEGERAL;
}

bool constant_pair_is_unsigned_integral(Constant *cons1, Constant *cons2) {
  return cons1->type == CONTYPE_UNSIGNED_INTEGERAL
	  	&& cons2->type == CONTYPE_UNSIGNED_INTEGERAL;
}

bool constant_pair_is_rational(Constant *cons1, Constant *cons2) {
  return cons1->type == CONTYPE_RATIONAL
	  	&& cons2->type == CONTYPE_RATIONAL;
}

bool constant_pair_is_boolean(Constant *cons1, Constant *cons2) {
  return cons1->type == CONTYPE_BOOLEAN
	  	&& cons2->type == CONTYPE_BOOLEAN;
}

Result *create_result(ResultType type, void *value) {
  return (Result *)create_variable(type, value);
}

void free_variable(Variable *op) {
  if (op == NULL)
    return;

  if (op->type == CONTYPE_PTR && op->ptr != NULL)
    FREE_AND_NULLIFY(&op->ptr);
  else if (op->type == CONTYPE_STR && op->str != NULL)
    FREE_AND_NULLIFY(&op->str);

  FREE_AND_NULLIFY(&op);
}

void free_instruction(Instruction *inst) {
  while (--inst->num_variables)
    free_variable(inst->variables[inst->num_variables]);

  free_variable(inst->result);

  FREE_AND_NULLIFY(&inst);
}
