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
};

/* Declarations for syntax-directed translation types and functions, see `geezer-sdt.c` for definitions */

typedef void (*SdtCallbackFn)(int info, char *term);
typedef struct SymbolsTable SymbolsTable;


uint32_t djb2_hash(char *message);
void symbtable_insert(SymbolsTable **tab, char *key, void *value);
char *symtable_get(SymbolsTable *tab, char *key);
void symtable_dump(SymbolsTable *tab);


void walk_and_translate(ASTNode *root, SdtCallbackFn callbackfn);


#define DJB2_MAGIC	 33

#endif
