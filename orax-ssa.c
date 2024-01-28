#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "orax-decl.h"

struct SSAVariable {
	Operand *operand;
	version_t version;
};
