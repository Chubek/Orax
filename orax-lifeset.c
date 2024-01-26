#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "orax-decl.h"



struct LifeField
{
   uint16_t op1 : 1;
   uint16_t op2 : 1;
   uint16_t op3 : 1;
   uint16_t op4 : 1;
   uint16_t op5 : 1;
   uint16_t op6 : 1;
   uint16_t op7 : 1;
   uint16_t op8 : 1;
   uint16_t op9 : 1;
   uint16_t op10 : 1;
   uint16_t op11 : 1;
   uint16_t op12 : 1;
   uint16_t op13 : 1;
   uint16_t op14 : 1;
   uint16_t op15 : 1;
   uint16_t op16 : 1;
};

struct LifeSet
{
  LifeField **fields;
  size_t num_fields;
};


LifeField *new_life_field(void)
{
  LifeField *field = (LifeField*)calloc(1, sizeof(LifeField));
  return field;
}


void set_life_field_index(LifeField *field, size_t index, bool value)
{
   *field &= ~(1 << index);
   *field |= (value & 1) << index;
}

bool fields_are_equal(LifeField *field1, LifeField *field2)
{
  uint16_t field1_int = (uint16_t)(*field1);
  uint16_t field2_int = (uint16_t)(*field2);
  return field1_int == field2_int;
}

LifeSet *create_life_set(void)
{
  LifeSet *set = (LifeSet*)calloc(1, sizeof(LifeSet));
  set->fields = NULL;
  set->num_fields = 0;
  return set;
}

void set_life_set_field_at(LifeField *field, size_t field_index, size_t field_at, bool value)
{
  set_life_field_index(set->fields[field_index], field_at, value);
}

LifeSet *add_life_set_empty_field(LifeSet *set)
{
  set->fields = 
     (LifeField**)realloc(set->fields, (set->num_fields + 1) * sizeof(LifeSet));
  set->fields[set->num_fields++] = new_life_field();
  return set;
}

LifeSet *add_life_set_field(LifeSet *set, LifeField *field)
{
  set->fields = 
     (LifeField**)realloc(set->fields, (set->num_fields + 1) * sizeof(LifeSet));
  set->fields[set->num_fields++] = field;
  return set;
}


LifeSet *union_life_set(LifeSet *set1, LifeSet *set2)
{
   LifeSet *set_union = create_life_set();

   for (size_t i = 0; i < set1->num_fields; i++)
      set_union = add_life_set_field(set1->fields[i]);

   for (size_t i = 0; i < set2->num_fields; i++)
   {
       bool is_dup = false;

       for (size_t j = 0; j < set_union->num_fields; j++)
       {
	 if (fields_are_equal(set2->fields[i], union_set->fields[j]))
 	 {
	    is_dup = true;
	    break;
	 }
       }

       if (!is_dup)
	set_union = add_life_set_field(set2->fields[i]);
   }

   return set_union;
}


LifeSet *difference_life_set(LifeSet *set1, LifeSet *set2)
{
   LifeSet *set_diff = create_life_set();

   for (size_t i = 0; i < set1->num_fields; i++)
   {
	bool is_present = false;

	for (size_t j = 0; j < set2->num_fields; j++)
	{
	   if (fields_are_equal(set1->fields[i], set2->fields[j]))
	   {
	      is_preset = true;
	      break;
	   }
	}

	if (!is_present)
	  set_diff = add_life_set(set1->fields[i]);
   }
   return set_diff;
}


LifeSet *interset_life_set(LifeSet *set1, LifeSet *set2)
{
  LifeSet *set_inter = create_life_set();

  for (size_t i = 0; i < set1->num_fields; i++)
   {
	bool is_present = false;

	for (size_t j = 0; j < set2->num_fields; j++)
	{
	   if (fields_are_equal(set1->fields[i], set2->fields[j]))
	   {
	      is_preset = true;
	      break;
	   }
	}

	if (is_present)
	  set_inter = add_life_set(set1->fields[i]);
   }
   return set_inter;
}









