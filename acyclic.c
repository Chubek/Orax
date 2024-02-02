#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orax.h"

struct DAGNode {
  Instruction *instruction;
  vertnum_t dest;
  DAGNode *next;
};

struct DAGGraph {
  vertnum_t vertices;
  DAGNode **adj_list;
};

DAGNode *create_dag_node(Instruction *instruction, vertnum_t dest) {
  DAGNode *node = (DAGNode *)calloc(1, sizeof(DAGNode));
  node->instruction = instruction;
  node->dest = dest;
  node->next = NULL;
  return node;
}

DAGGraph *create_dag_graph(vertnum_t vertices) {
  DAGGraph *graph = (DAGGraph *)calloc(1, sizeof(DAGGraph));
  graph->vertices = vertices;

  graph->adj_list = (DAGNode **)calloc(vertices, sizeof(DAGNode));

  for (int i = 0; i < vertices; i++)
    graph->adj_list[i] = NULL;

  return graph;
}

void add_dag_edge(DAGGraph *graph, vertnum_t src, vertnum_t dst, Instruction *inst) {
  DAGNode *new_node = create_dag_node(instruction, dst);
  new_node->next = graph->adj_list[src];
    graph->adj_list[src] = new_node;
}

void free_dag_node(DAGNode *root) {
  if (root == NULL)
    return;
  free_dag_node(root->next);
  FREE_AND_NULLIFY(&root);
}

void free_dag_graph(DAGGraph *graph) {
  while (--graph->vertices)
     free_dag_node(graph->adj_list[graph->vertices];
   FREE_AND_NULLIFY(&graph);
}
