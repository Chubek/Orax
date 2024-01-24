#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "zcc-decl.h"

/* A DAG (Directed Acyclic Graph) is a type of graph which is loop-less, and each edge has a 
 * specific direction.
 * Take this for example:
 * 1 ---> 4 <--- 5
 * |		 |
 * v 		 v
 * 3 ---> 2 ---> 6
 * Here, vertices are represented by numbers and edges are represented by lines. The vertical pipes
 * and dashed lines represent edges, and the numbers represent the vertext number.
 * This graph, simple as it is, is a textbook definition of a DAG. It is loop-less, and every edge 
 * has a specific direction
 * -------
 * Below, we define the various data structures we need, the DAG node, the acyclic graph 
 * using adjacency lists (despite CSLR saying adjacency matrices are a better option, I still want 
 * this to be simple) and the structure for data which DAG nodes hold
 */

struct DAGNode
{
  DAGData *data;  // The data for the node
  vert_t dest;	  // The index for the destination to the end of the 'arrow' directed to this node
  DAGNode *next;  // The next graph in the adjacency list
};

struct DAGGraph
{
  vert_t vertices;	// Number of vertices this graph holds
  DAGNode **adj_list;	// The adjacency list that defines this graph
};

struct DAGData
{
  /* TODO */
};

/* Below are the factory functions for the DAG structures */

DAGNode *create_dag_node(DAGData *data, vert_t dest)
{
  DAGNode *node = (DAGNode*)calloc(1, sizeof(DAGNode));
  node->data = data;
  node->dest = dest;
  node->next = NULL;
  return node;
}

DAGGraph *create_dag_graph(vert_t vertices)
{
  DAGGraph *graph = (DAGGraph*)calloc(1, sizeof(DAGGraph));
  graph->vertices = vertices;

  graph->adj_list = (DAGNode**)calloc(vertices, sizeof(DAGNode));

  for (int i = 0; i < vertices; i++)
     graph->adj_list[i] = NULL;

  return graph;
}

void add_dag_edge(DAGGraph **graph, vert_t src, vert_t dst, DAGData *data)
{
  DAGNode *new_node = create_dag_node(data, dst);
  new_node->next = (*graph)->adj_list[src];
  (*graph)->adj_list[src] = new_node;
}

DAGData *create_dag_data(DAGDataType type, char *value)
{
  DAGData *data = (DAGData*)calloc(1, sizeof(DAGData));
  data->type = type;
  data->value = value;
  return data;
}

void free_dag_node(DAGNode *root)
{
   DAGNode *node = NULL;
   while ((node = node->next) != NULL)
   {
     if (node->data != NULL)
       free(node->data);
     free(node);
   }
   if (root->data != NULL)
     free(root->data);
   free(root);
}

void free_dag_graph(DAGGraph *graph)
{
   while (--graph->vertices)
     free_dag_node(graph->adj_list[graph->vertices];
   free(graph);
}


