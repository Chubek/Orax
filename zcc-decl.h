#ifndef GEEZER_DECL_H_
#define GEEZER_DECL_H_

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

// + A: The Abstract Syntax Tree +

/* Declaration of AST structure types and functions, see `zcc-ast.c` for definitions */

typedef enum ASTNodeType ASTNodeType;	// Defined in `zcc-enums.h`
typedef enum ASTLeafType ASTLeafType;	// Defined in `zcc-enums.h`
typedef struct ASTNode ASTNode;
typedef struct ASTLeaf ASTLeaf;

ASTNode *create_ast_node(ASTNodeType type);
ASTLeaf *create_ast_leaf(ASTNodeType type, void *value);
ASTNode *add_ast_left_subtree(ASTNode *node, ASTNode *subtree);
ASTNode *add_ast_right_subtree(ASTNode *node, ASTNode *subtree);
ASTNode *add_ast_leaf(ASTNode *node, ASTLeaf *leaf);
void free_ast_node(ASTNode *root);
void free_ast_leaf(ASTLeaf *leaf);


// + B: The Symbols Table +

/* Type definitions and function declarations for the symbols table, see `zcc-symtable.c` for definitions */

typedef struct SymbolsTable SymbolsTable;

uint32_t djb2_hash(char *message);
void symbtable_insert(SymbolsTable **tab, char *key, void *value);
char *symtable_get(SymbolsTable *tab, char *key);
void symtable_dump(SymbolsTable *tab);

// + C: The Directed Acyclic Graph +

/* DAG declarations and typedefs, see `zcc-dag.c for more definitions */

typedef struct Instruction Instruction; // This will be re-declared in the next section
typedef struct DAGNode DAGNode;
typedef struct DAGGraph DAGGraph;
typedef int vert_t;

DAGNode *create_dag_node(Instruction *inst, vert_t dest);
DAGGraph *create_dag_graph(vert_t vertices);
void add_dag_edge(DAGGraph **graph, vert_t src, vert_t dst, Instruction *inst);

void free_dag_node(DAGNode *root);
void free_dag_graph(DAGGraph *graph);

// + D: The Instructions +

/* The following functions and typedefs declare the instructions. See `zcc-inst.c` for definitions */

typedef enum InstructionType InstructionType;	// Defined in `zcc-enums.h`
typedef enum OperandType OperandType;		// Defined in `zcc-enums.h`
typedef enum OperandType ResultType;		// Defined in `zcc-enums.h`
typedef struct Instruction Instruction;
typedef struct Operand Operand;
typedef struct operand Result;
typedef int instid_t;

Instruction *create_instruction(InstructionType type, instid_t instruction_id);
Instruction *add_inst_operand(Instruction *inst, Operand *operand);
Instruction *add_inst_result(Instruction *inst, Result *result);

Operand *create_operand(OperandType type, void *value);
Result *create_result(ResultType type, void *value);


// + E: The Control Flow Graph +

/* The following functions declares a Basic Block used for Control Flow Analysis. See `zcc-cfg.c` for definitions */

typedef struct BasicBlock BasicBlock;
typedef int succid_t;
typedef int blockid_t;

BasicBlock *create_basic_block(blockid_t block_id);
BasicBlock *add_cfg_successor(BasicBlock *block, succid_t successor_id);
BasicBlock *add_cfg_instruction(BasicBlock *block, Instruction *inst);




#endif
