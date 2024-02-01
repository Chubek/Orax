#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "orax-munch.h"


GENFN walk_munch_node(MaxMunchState *state, MunchNode *node) {
    if (node == NULL)
	    return;

    if (node->leaf_type == LEAF_IS_NOT) {
	    switch (node->type) {
		case MUNCH_TREE:
			walk_munch_node(node->left);
			walk_munch_node(node->right);
			break;
	    }
    }
  
}
