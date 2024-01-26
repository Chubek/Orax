#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


#include "zcc-decl.h"


struct  RegisterNode
{
  regid_t id;
  char *name;
  color_t color;
  degree_t degree;
  RegisterNode *neighbors[MAX_NEIGHBORS];
};



RegisterNode *create_register(regid_t id, char *name)
{
  RegisterNode *node = (Node*)calloc(1, sizeof(RegisterNode));
  node->id = id;
  node->name = name;
  node->color = COLOR_INIT;
  node->degree = DEGREE_INIT;
  node->num_neighbors = 0;
  return node;
}


void update_node_degree(RegisterNode *node)
{
  node->degree = 0;
  for (size_t i = 0; i < MAX_NEIGHBORS; i++)
  {
     if (node->neighbors[i] != NULL)
       node->degree++;
  }
}

void remove_neighbor(RegisterNode *node, degree_t neighbor_degree)
{
  node->neighbors[neighbor_degree] = NULL;
  update_node_degree(node);
}

void remove_all_neighbors(RegisterNode *node)
{
  for (size_t i = 0; i < MAX_NEIGHBORS; i++)
    remove_neighbors(node, i);
  node->color = COLOR_REMOVED;
}

void add_register_edge(RegisterNode *node1, RegisterNode *node2)
{
   node1->neighbors[node1->id - 1] = node2;
   node2->neighbors[node2->id - 1] = node1;
   update_node_degree(node1);
   update_node_degree(node2);
}


RegisterNode *get_node_with_least_degree(RegisterNode *nodes[], size_t num_nodes)
{
   degree_t minimum_degree = 0;
   RegisterNode *minumum_degree_node = NULL;
   
   for (size_t i = 0; i < num_nodes; i++)
   {
       if (NODE_IS_COLORED(nodes[i]))
	continue;
       update_node_degree(nodes[i]);
       if (nodes[i]->degree < minimum_degree)
       {
	  minimum_degree = nodes[i]->degree;
	  minimum_degree_node = nodes[i];
       }	
   }
   
   return minimum_degree_node;
}

void simplify_registers(RegisterNode *nodes[], size_t num_nodes)
{
  size_t removed_nodes = 0;

  while (removed_nodes < num_nodes)
  {
    RegisterNode *minimum_degree_node = 
	get_node_with_least_degree(&nodes[removed_nodes], num_nodes - removed_nodes);

     if (minimum_degree_node == NULL)
      return;
     else
     {
	remove_all_neighbors(minimum_degree_node);
	removed_nodes++;
     }
  }

}


void spill_eligible_to_memory(RegisterNode *nodes[], size_t num_nodes)
{
  for (size_t i = 0; i < num_nodes; i++)
  {
     update_node_degree(nodes[i]);

     if (nodes[i]->degree >= MAX_SPILLABLE)
       node[i]->color = COLOR_SPILLED;
  }
}


void color_graphs(RegisterNode *nodes[], size_t num_nodes)
{
  for (size_t i = 0; i < num_nodes; i++)
  {
     update_node_degree(nodes[i]);

     if (NODE_IS_COLORED(nodes[i]))
      continue;

     color_t used_colors[MAX_NEIGHBORS] = {false};

     for (size_t j = 0; j < MAX_NEIGHBORS; j++)
        if (nodes[i]->neighbors[j] != NULL)
	  used_colors[nodes[i]->neighbors[j]->color] = true;
    
     for (nodes[i]->color = 0; nodes[i]->color < MAX_NEIGHBORS; nodes[i]->color++)
	if (!used_colors[nodes[i]->color])
	 break; 
  }
}


void select_registers(RegisterNode *nodes[], size_t num_nodes)
{
  for (size_t i = 0; i < num_nodes; i++)
  {
    if (NODE_IS_COLOR(nodes[i]))
      continue;

           
  }
}


