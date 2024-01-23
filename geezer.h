#ifndef GEEZER_H_
#define GEEZER_H_


typedef enum ASTType ASTType;
typedef enum ASTNode ASTNode;


ASTNode* new_ast_node(ASTType type);
ASTNode* new_ast_leaf(ASTType type, char* value);











#endif
