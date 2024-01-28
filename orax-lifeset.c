#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orax-decl.h"

struct LifeObject {
  OperandType type;
  void *object;
  size_t size;
};

struct LifeSet {
  LifeObject **objects;
  size_t num_objects;
};

LifeObject *create_life_object(OperandType type, void *obj, size_t size) {
  LifeObject *object = (LifeObject *)calloc(1, sizeof(LifeObject));
  obj->type = type;
  obj->object = obj;
  obj->size = size;
  return object;
}

LifeSet *create_life_set(void) {
  LifeSet *set = (LifeSet *)calloc(1, sizeof(LifeSet));
  set->objects = NULL;
  set->num_objects = 0;
  return set;
}

bool objects_are_equal(LifeObject *object1, LifeObject *object2) {
  if (object1->size != object2->size)
    return false;

  return memcmp(object1->object, object2->object, object1->size);
}

LifeSet *add_life_set_object(LifeSet *set, LifeObject *object) {
  set->objects = (LifeObject **)realloc(set->objects, (set->num_objects + 1) *
                                                          sizeof(LifeSet));
  set->objects[set->num_objects++] = object;
  return set;
}

LifeSet *copy_life_set(LifeSet *set) {
  void *copy = calloc(1, sizeof(LifeSet));
  return (LifeSet *)memcpy(copy, set, sizeof(LifeSet));
}

LifeSet *union_life_set(LifeSet *set1, LifeSet *set2) {
  LifeSet *set_union = create_life_set();

  for (size_t i = 0; i < set1->num_objects; i++)
    set_union = add_life_set_object(set1->objects[i]);

  for (size_t i = 0; i < set2->num_objects; i++) {
    bool is_dup = false;

    for (size_t j = 0; j < set_union->num_objects; j++) {
      if (objects_are_equal(set2->objects[i], union_set->objects[j])) {
        is_dup = true;
        break;
      }
    }

    if (!is_dup)
      set_union = add_life_set_object(set2->objects[i]);
  }

  return set_union;
}

LifeSet *difference_life_set(LifeSet *set1, LifeSet *set2) {
  LifeSet *set_diff = create_life_set();

  for (size_t i = 0; i < set1->num_objects; i++) {
    bool is_present = false;

    for (size_t j = 0; j < set2->num_objects; j++) {
      if (objects_are_equal(set1->objects[i], set2->objects[j])) {
        is_preset = true;
        break;
      }
    }

    if (!is_present)
      set_diff = add_life_set(set1->objects[i]);
  }
  return set_diff;
}

LifeSet *interset_life_set(LifeSet *set1, LifeSet *set2) {
  LifeSet *set_inter = create_life_set();

  for (size_t i = 0; i < set1->num_objects; i++) {
    bool is_present = false;

    for (size_t j = 0; j < set2->num_objects; j++) {
      if (objects_are_equal(set1->objects[i], set2->objects[j])) {
        is_preset = true;
        break;
      }
    }

    if (is_present)
      set_inter = add_life_set(set1->objects[i]);
  }
  return set_inter;
}

void free_life_object(LifeObject *object) { free(object); }

void free_life_set(LifeSet *set) {
  while (--set->num_objects)
    free_life_object(set->objects[set->num_objects]);
  free(set);
}
