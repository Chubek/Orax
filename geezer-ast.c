#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "geezer.h"

/* The following data structues define Abstract Syntax Tree for the Geezer langauge.
 * Geezer is a language used to store data about packages, libraries, and it provides
 * ways to define commands to install them.
 * When the `geezer(1)` command is invoked, the Flex lexer scans the passed document
 * and passes it to the Yacc parser which in turn translates the document into the following AST.
 * This AST, then, is used to construct an executable binary which can be invoked to print data
 * about libraries and packages, or install them
 */


/* The ASTType enumeration stores the types of AST nodes. */

typedef enum ASTType
{
  COMMAND_IDENTIFIER,	// An identifier for "defcmd"
  PACKAGE_IDENTIFIER,	// An identifier for "defpkg"
  INSTALL_IDENTIFIER,	// An identifier for "defins"
  INVOKE_IDENTIFIER,	// A provoked command identifier, prefixed with '%'
  COMMAND_TEXT,		// The command text, a list of shell words terminated by ';'
  INFO_TEXT,		// Information singleton for the package (can be many)
  COMMAND_PARAMETER,	// Parameters defined for the command (prefixe with '$' in COMMAND_TEXT)
  INVOKE_ARGUMENT,	// The shell word passed as an argument to INVOKE_IDENTIFIER, replaces '$'s1
  SUBST_PARAM,		// The parameter to be substituted
}
ASTType;


/* The ASTNode structure stores an AST node, or leaf */

typedef struct ASTNode
{
  ASTType type;				// The type
  struct ASTNode *left, *right;		// Left and right subtree
  bool is_leaf;				// Is this a leaf? Leaves represent terminals
  char *value;				// This only will be non-NULL if leaf
} 
ASTNode;


/* The following function creates an AST node */

ASTNode *new_ast_node(ASTType type)
{
   ASTNode *node = (ASTNode*)calloc(1, sizeof(ASTNode));
   node->type = type;
   return node;
}

/* The following function creates an AST leaf */

ASTNode *new_ast_leaf(ASTType type, char *value)
{
  ASTNode *node = new_ast_node(type);
  node->is_leaf = true;
  node->value = valeu;
  return node;
}
