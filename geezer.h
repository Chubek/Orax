#ifndef GEEZER_H_
#define GEEZER_H_


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



#endif
