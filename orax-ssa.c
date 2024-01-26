#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>


#include "orax-decl.h"


struct SSAVariable
{
  Operand *operand;
  version_t version;
};

struct SSADefUse
{
  SSAVariable *variable;
  TraceBlock **used_in;
  size_t num_used_in;
};

struct SSAUseDef
{
  SSAVariable *variable;
  TraceBlock **reaches;
  size_t num_reaches;
};

struct SSAGraphNode
{
  SSADefUse **defs;
  size_t num_defs;
  SSAUseDef **uses;
  size_t num_uses;
  SSGraphNode **adjacent;
  size_t num_adjacent;
};


struct SSAGraph
{
  SSAGraphNode **nodes;
  size_t num_nodes;
};


SSAVariable *create_ssa_variable(Operand *operand)
{
   SSAVariable *var = (SSAVariable*)calloc(1, sizeof(SSAVariable));
   var->operand = operand;
   var->version = VERSION_INIT;
   return var;
}

SSADefUse *create_ssa_def_use(SSAVariable *var)
{
   SSADefUse *def = (SSADefUse*)calloc(1, sizeof(SSADefUse));
   def->variable = var;
   def->used_in = NULL;
   return def;
}

SSAUseDef *create_ssa_use_def(SSAVariable *var)
{
   SSADefUse *use = (SSADefUse*)calloc(1, sizeof(SSAUseDef));
   use->variable = var;
   use->reaches = NULL;
   return use;
}

SSAGraphNode *create_ssa_graph_node(void)
{
   SSAGraphNode *node = (SSAGraphNode*)calloc(1, sizeof(SSAGraphNode));
   node->defs = NULL;
   node->uses = NULL;
   node->adjacent = NULL;
   return node;
}

SSAGraph *create_ssa_graph(void) {
  SSAGraph *graph = (SSAGraph*)calloc(1, sizeof(SSAGraph));
  graph->nodes = NULL;
  graph->num_nodes = 0;
  return graph;
}



