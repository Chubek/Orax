#ifndef GEEZER_H_
#define GEEZER_H_

/* Declaration of AST structure types and functions, see `geezer-ast.c` for definitions */

typedef enum ASTType ASTType;
typedef enum ASTNode ASTNode;


ASTNode *new_ast_node(ASTType type);
ASTNode *new_ast_info(char *id, char *text);
ASTNode *new_ast_cmd(char *id, ASTNode *params, ASTNode *command);
ASTNode *new_ast_inst(char *id, ASTNode *invoke);
ASTNode *new_ast_cmdident(char *value);
ASTNode *new_ast_ellipse(void);
ASTNode *new_ast_shword(char *value);
ASTNode *new_ast_cmdtxt(char *value);
ASTNode *new_ast_refident(char *value);
ASTNode *ast_add_child(ASTNode **node, ASTNode *child);
void free_ast(ASTNode *root);

/* The ASTType enumeration stores the types of AST nodes. */

enum ASTType
{
  AST_NODE_PKGDEF,
  AST_NODE_CMDDEf,
  AST_NODE_INSTDEF,
  AST_NODE_INFO,
  AST_LEAF_CMDIDENT,
  AST_LEAF_ELLIPSE,
  AST_LEAF_SHWORD,
  AST_LEAF_CMDTXT,
  AST_LEAF_REFIDENT,
  AST_LEAF_INFOIDENT,
  AST_LEAF_INFOTXT,
  AST_LEAF_INSTIDENT,
};



#endif
