#ifndef GEEZER_DECL_H_
#define GEEZER_DECL_H_

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

// + A: The Abstract Syntax Tree +

/* Declaration of AST structure types and functions, see `zcc-ast.c` for definitions */

typedef enum ASTNodeType ASTNodeType;
typedef enum ASTLeafType ASTLeafType;
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

typedef struct DAGNode DAGNode;
typedef struct DAGGraph DAGGraph;
typedef struct DAGData DAGData;
typedef int vert_t;

DAGNode *create_dag_node(DAGData *data, vert_t dest);
DAGGraph *create_dag_graph(vert_t vertices);
void add_dag_edge(DAGGraph **graph, vert_t src, vert_t dst);
DAGData *create_dag_data(/* TODO */);

void free_dag_node(DAGNode *root);
void free_dag_graph(DAGGraph *graph);

// + D: The Control Flow Graph +

/* The following functions declares a Basic Block used for Control Flow Analysis. See `zcc-cfg.c` for definitions */

typedef struct BasicBlock BasicBlock;
typedef int succid_t;
typedef int blockid_t;

BasicBlock *create_basic_block(blockid_t block_id);
BasicBlock *add_cfg_successor(BasicBlock *block, succid_t successor_id);



 


#endif
