#ifndef GEEZER_H_
#define GEEZER_H_

#include <stdarg.h>
#include <stdint.h>

/* Declaration of AST structure types and functions, see `geezer-ast.c` for definitions */

typedef enum ASTType ASTType;
typedef enum ASTNode ASTNode;


ASTNode *new_ast_node(ASTType type);
ASTNode *new_ast_info(char *id, char *text);
ASTNode *new_ast_pkg(char *id, ASTNode *body);
ASTNode *new_ast_cmd(char *id, ASTNode *params, ASTNode *command);
ASTNode *new_ast_inst(char *id, ASTNode *invoke);
ASTNode *new_ast_cmdident(char *value);
ASTNode *new_ast_invoke(char *id, ASTNode *args);
ASTNode *new_ast_ellipse(void);
ASTNode *new_ast_shword(char *value);
ASTNode *new_ast_cmdtxt(char *value);
ASTNode *new_ast_refident(char *value);
ASTNode *new_ast_terminate(void);
ASTNode *ast_add_child(ASTNode **node, ASTNode *child);
void free_ast(ASTNode *root);

/* The ASTType enumeration stores the types of AST nodes. */

enum ASTType
{
  AST_NODE_PKGDEF,
  AST_NODE_CMDDEF,
  AST_NODE_INSTDEF,
  AST_NODE_INFO,
  AST_NODE_INVOKE,
  AST_LEAF_CMDIDENT,
  AST_LEAF_ELLIPSE,
  AST_LEAF_SHWORD,
  AST_LEAF_CMDTXT,
  AST_LEAF_REFIDENT,
  AST_LEAF_INFOIDENT,
  AST_LEAF_INFOTXT,
  AST_LEAF_INSTIDENT,
  AST_LEAF_TERMINATE,
  AST_LEAF_PKGIDENT,
  AST_LEAF_INVOKEIDENT,
  AST_LEAF_PARAM,
};


/* Type definitions and function declarations for the symbols table, see `geezer-symtable.c` for definitions */

typedef struct SymbolsTable SymbolsTable;

uint32_t djb2_hash(char *message);
void symbtable_insert(SymbolsTable **tab, char *key, void *value);
char *symtable_get(SymbolsTable *tab, char *key);
void symtable_dump(SymbolsTable *tab);


/* DAG, and callback functions for SDT, see `geezer-dag.c for more info */

typedef struct DAGNode DAGNode;
typedef struct DAGGraph DAGGraph;
typedef struct DAGData DAGData;
typedef enum DAGDataType DAGDataType;
typedef int vert_t;

DAGNode *create_dag_node(DAGData *data, vert_t dest);
DAGGraph *create_dag_graph(vert_t vertices);
void add_dag_edge(DAGGraph **graph, vert_t src, vert_t dst);
DAGData *create_dag_data(DAGDataType type, char *value);

void free_dag_node(DAGNode *root);
void free_dag_graph(DAGGraph *graph);

void callback_yaf_languages(tflag_t flag, char *term);


/* This enumeration contains the types of data which a DAG may contain */

enum DAGDataType
{
  DAG_FUNCTION,
  DAG_VARIABLE,
  DAG_STRING_LITERAL,
  DAG_INTEGER_LITERAL,
  DAG_WHILE_LOOP,
  DAG_FOR_LOOP,
  DAG_FOREACH_LOOP,
  DAG_LIST_LITERAL,
  DAG_HASHMAP_LITERAL,
  DAG_IF_COND,
  DAG_ELSE_COND,
  DAG_CLASS,
  DAG_STRUCT,
  DAG_ENUM,
};




#endif
