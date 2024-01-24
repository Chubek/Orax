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

/* Declarations for syntax-directed translation types and functions, see `geezer-sdt.c` for definitions */

typedef void (*SdtCallbackFn)(int flag, char *term);
typedef struct SymbolsTable SymbolsTable;


uint32_t djb2_hash(char *message);
void symbtable_insert(SymbolsTable **tab, char *key, void *value);
char *symtable_get(SymbolsTable *tab, char *key);
void symtable_dump(SymbolsTable *tab);


void walk_and_translate(ASTNode *root, SdtCallbackFn callbackfn);

void sdt_to_posix_shell(int flag, char *term);

#define DJB2_MAGIC	 33

#define FLAG_SET_MODE	 (1 << 1)
#define FLAG_UNSET_MODE	 (1 << 2)
#define FLAG_MODE_CMD	 (1 << 3)
#define FLAG_MODE_PKG	 (1 << 4) 
#define FLAG_MODE_INST	 (1 << 5)
#define FLAG_MODE_INFO	 (1 << 6)
#define FLAG_MODE_INVOKE (1 << 7)
#define FLAG_LEAF_MODE	 (1 << 8)
#define FLAG_IDENT	 (1 << 9)
#define FLAG_VARPARAM	 (1 << 10)
#define	FLAG_PARAM	 (1 << 11)
#define FLAG_REFIDENT	 (1 << 12)
#define FLAG_SHELLWORD	 (1 << 13)



#endif
