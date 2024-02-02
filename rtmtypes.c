#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orax-decl.h"
#include "orax-enums.h"

struct SingletonType {
  TypeKind kind;
  BaseWordType baseword;
  TypeStaticity staticity;
  TypeStrength strength;
  TypeFields *fields;
  TypeEnumerationFields *enumeration_fields;
  TypeVirtualTable *vtable;
  typesize_t size;
  typeid_t id;
};

struct TypeFields {
  SingletonType **singletons;
  size_t num_singletons;
};

struct TypeEnumerationFields {
  TypeEnumeration **enumerations;
  size_t num_enumerations;
};

struct TypeEnumeration {
  char *name;
  void **values;
  size_t num_values;
};

struct TypeVirtualTable {
  TypeMethod **methods;
  size_t num_methods;
};

SingletonType *create_singleton_type(TypeKind kind, TypeStaticity staticity,
                                     TypeStrength strength) {
  SingletonType *singleton = (SingletonType *)calloc(1, sizeof(SingletonType));
  singleton->kind = kind;
  singleton->staticity = staticity;
  singleton->strength = strength;
  singleton->fields = NULL;
  singleton->enumeration_fields = NULL;
  singleton->vtable = NULL;
  return singleton;
}

SingletonType *add_singleton_type_fields(SingletonType *singleton) {
  singleton->fields = create_type_fields();
}

SingletonType *add_singleton_type_enumeration_fields(SingletonType *singleton) {
  singleton->fields = create_type_enumeration_fields();
}

SingletonType *add_singleton_type_vtable(SingletonType *singleton) {
  singleton->fields = create_type_vtable();
}

TypeFields *create_type_fields(void) {
  TypeFields *fields = (TypeFields *)calloc(1, sizeof(TypeFields));
  return fields;
}

TypeEnumerationFields *create_type_enumeration_fields(void) {
  TypeEnumerationFields *enumeration_fields =
      (TypeEnumerationFields *)calloc(1, sizeof(TypeEnumerationFields));

  return enumeration_fields;
}

TypeVirtualTable *create_type_vtable(void) {
  TypeVirtualTable *vtable =
      (TypeVirtualTable *)calloc(1, sizeof(TypeVirtualTable));
  return vtable;
}

SingletonType *add_singleton_field(SingletonType *singleton,
                                   SingletonType *field) {
  singleton->fields->singletons = (SingletonType **)realloc(
      singleton->fields->singletons,
      (singleton->fields->num_singletons + 1) * sizeof(SingletonType *));
  singleton->fields.singletons[singleton->fields->num_singletons++] = field;
  return singleton;
}

SingletonType *add_singleton_enumeration_field(SingletonType *singleton,
                                               TypeEnumeration *enumeration) {
  singleton->enumeration_fields->enumerations = (TypeEnumeration **)realloc(
      singleton->enumeration_fields->enumerations,
      (singleton->enumeration_fields->num_enumerations + 1) *
          sizeof(TypeEnumeration *));
  singleton->enumeration_fields
      .enumerations[singleton->enumeration_fields->num_enumerations++] =
      enumeration;
  return singleton;
}

SingletonType *add_singleton_vtable_method(SingletonType *singleton,
                                           TypeMethod *meth) {
  singleton->vtable->methods = (TypeMethod **)realloc(
      singleton->vtable->methods,
      (singleton->vtable->num_methods + 1) * sizeof(TypeMethod *));
  singleton->vtable->methods[singleton->vtable->num_methods++] = meth;
  return singleton;
}
