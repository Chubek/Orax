#ifndef ORAX_DECL_H_
#define ORAX_DECL_H_

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// + A: The Abstract Syntax Tree +

/* Declaration of AST structure types and functions, see `orax-ast.c` for
 * definitions */

typedef enum ASTNodeType ASTNodeType; // Defined in `orax-enums.h`
typedef enum ASTLeafType ASTLeafType; // Defined in `orax-enums.h`
typedef struct ASTNode ASTNode;
typedef struct ASTLeaf ASTLeaf;

ASTNode *create_ast_node(ASTNodeType type);
ASTLeaf *create_ast_leaf(ASTNodeType type, void *value);
ASTNode *add_ast_left_subtree(ASTNode *node, ASTNode *subtree);
ASTNode *add_ast_right_subtree(ASTNode *node, ASTNode *subtree);
ASTNode *add_ast_leaf(ASTNode *node, ASTLeaf *leaf);
void free_ast_node(ASTNode *root);
void free_ast_leaf(ASTLeaf *leaf);

// + B: The Symbols Table +

/* Type definitions and function declarations for the symbols table, see
 * `orax-symtable.c` for definitions */

typedef struct SymtabNode SymtabNode;

uint32_t djb2_hash(char *message);
void symbtable_insert(SymtabNode **tab, char *key, void *value);
void *symtable_get(SymtabNode *tab, char *key);
void free_symtable(SymtabNode *tab);

// + C: The Directed Acyclic Graph +

typedef struct Instruction
    Instruction; // This will be re-declared in the next section
typedef struct DAGNode DAGNode;
typedef struct DAGGraph DAGGraph;
typedef int vertnum_t;

DAGNode *create_dag_node(Instruction *inst, vertnum_t dest);
DAGGraph *create_dag_graph(vertnum_t vertices);
void add_dag_edge(DAGGraph *graph, vertnum_t src, vertnum_t dst, Instruction *inst);

void free_dag_node(DAGNode *root);
void free_dag_graph(DAGGraph *graph);

// + D: The Instructions +

typedef enum InstructionName InstructionName; // Defined in `orax-enums.h`
typedef enum OperandType OperandType;		// Defined in `orax-enums.h`
typedef enum VariableType VariableType;         // Defined in `orax-enums.h`
typedef enum OperandType ResultType;          // Defined in `orax-enums.h`
typedef enum ConstantType ConstantType;		// Defined in `orax-enums.h`
typedef struct SSAInfo SSAInfo;
typedef struct Instruction Instruction;
typedef struct Operand Operand;
typedef struct Variable Variable;
typedef struct Operand Result;
typedef int instid_t;
typedef int varhash_t;
typedef int ssaversion_t;
typedef size_t typesize_t;
typedef unsigned long long unsigned_integral_t;
typedef signed long long signed_integral_t;
typedef long double rational_t;
typedef void *memory_pointer_t;
typedef bool boolean_t;

Instruction *create_instruction(InstructionName name, InstructionClass class,
                                instid_t instruction_id);
Instruction *add_inst_variable(Instruction *inst, Variable *variable);
Instruction *add_inst_result(Instruction *inst, Result *result);
Variable *create_variable(varhash_t hash, VariableType type, void *value,
                        size_t size);
Result *create_result(ResultType type, void *value);
Variable *duplicate_variable(Variable *op);
void add_to_variable_size(Variable *op, typesize_t addition);
void subtract_from_variable_size(Variable *op, typesize_t subtraction);
void free_variable(Variable *op);
void free_instruction(Instruction *inst);

#define SSA_VERSION_UNASSIGNED -1

// + E: The Control Flow Graph +

typedef enum TraceBlockType TraceBlockType; // Defined in `orax-enums.h`
typedef struct TraceBlock TraceBlock;
typedef struct ControlFlowGraph ControlFlowGraph;
typedef int blockid_t;

TraceBlock *create_trace_block(TraceBlockType type, blockid_t block_id,
                               const char *label);
ControlFlowGraph *create_cfg(void);
ControlFLowGraph *add_cfg_trace(ControlFlowGraph *cfg, TraceBlock *block);
TraceBlock *add_trace_successor(TraceBlock *block, TraceBlock *succ);
TraceBlock *add_trace_predecessor(TraceBlock *block, TraceBlock *pred);
TraceBlcok *add_trace_dominance_frontier(TraceBlock *block,
                                         TraceBlock *frontier);
TraceBlock *insert_trace_instruction_at_head(TraceBlock *block,
                                             Instruction *inst);
TraceBlock *add_trace_instruction(TraceBlock *block, Instruction *inst);
TraceBlock *get_successor_by_id(TraceBlock *block, blockid_t succ_bid);
Instruction *get_instruction_by_id(TraceBlock *block, instid_t instid);
bool trace_blocks_are_equal(TraceBlock *block1, TraceBlock *block2);
void ssa_calculate_immediate_dominator(ControlFlowGraph *cfg,
                                       size_t entry_index);
void ssa_calculate_dominance_frontiers(ControlFlowGraph *cfg);
void ssa_insert_phi_instructions(ControlFlowGraph *cfg);
void ssa_reversion_variables(ControlFlowGraph *cfg);
void analyze_liveness(ControlFlowGraph *graph);
bool dominates(TraceBlock *dominator, TraceBlock *block);
bool variable_defined_in_block(Variable *variable, TraceBlock *block);
bool is_phi_present_for_variable(TraceBlock *block, Variable *variable);
Instruction *create_instruction(TraceBlock *block, Variable *variable);
Variable *get_variable_from_block(Variable *variable, TraceBlock *block);
void insert_phi_instruction_at_head(TraceBlock *block, Instruction *inst);
void ssa_insert_phi_instructions(ControlFlowGraph *cfg);
void free_trace_block(TraceBlock *block);
void free_cfg(ControlFlowGraph *cfg);

// + F: The Interferrence Graph +

typedef struct RegisterNode RegisterNode;
typedef int regid_t;
typedef int color_t;
typedef int degree_t;

RegisterNode *create_register(regid_t id, char *name);
void update_node_degree(RegisterNode *node);
void remove_neighbor(RegisterNode *node, degree_t neighbor_degree);
void remove_all_neighbors(RegisterNode *node);
void add_register_edge(RegisterNode *node1, RegisterNode *node2);
RegisterNode *get_node_with_least_degree(RegisterNode *nodes[],
                                         size_t num_nodes);
void simplify_registers(RegisterNode *nodes[], size_t num_nodes);
void spill_eligible_to_memory(RegisterNode *nodes[], size_t num_nodes);
void select_registers(Node *nodes[], size_t num_nodes);
void coalesce_register_pair(RegisterNode *node1, RegisterNode *node2);
void color_registers(RegisterNode *nodes[], size_t num_nodes);
bool nodes_interfere(RegisterNode *node1, RegisterNode *node2);
void remove_node_from_graph(RegisterNode *nodes[], size_t num_nodes,
                            RegisterNode *to_remove);

#define DEGREE_INIT -1

#define COLOR_INIT -1
#define COLOR_REMOVED -2
#define COLOR_SPILLED -3

#define NODE_IS_COLORED(NODE) (NODE->color > 0)

#ifndef MAX_NEIGHBORS
#define MAX_NEIGHBORS 128
#endif

#ifndef MAX_SPILLABLE
#define MAX_SPILLABLE 8
#endif

// + G: The Lifeset +

/* These are declarations for the lifeset, see `orax-lifeset.c` for definitions
 */

typedef struct Variable LifeObject;
typedef struct LifeSet LifeSet;

LifeSet *create_life_set(void);
bool objects_are_equal(LifeObject *object1, LifeObject *object2);
LifeSet *add_life_set_object(LifeSet *set, LifeObject *object);
LifeSet *union_life_set(LifeSet *set1, LifeSet *set2);
LifeSet *difference_life_set(LifeSet *set1, LifeSet *set2);
LifeSet *interset_life_set(LifeSet *set1, LifeSet *set2);
LifeSet *copy_life_set(LifeSet *set);
void free_life_object(LifeObject *object);
void free_life_set(LifeSet *set);

// + H: Instruction Selection +

typedef enum MachineOpcode MachineOpcode;     // Generated from file
typedef enum MachineRegister MachineRegister; // Generated from file
typedef struct MachineInstruction MachineInstrction;
typedef struct InstructionTile InstructionTile;
typedef struct MaxMunchState MaxMunchState;
typedef int tileid_t;

MachineInstruction *create_machine_instruction(MachineOpcode opcode,
                                               MachineRegister dest_register,
                                               char *label, size_t line_number);
InstructionTile *create_instruction_tile(tileid_t id);
MaxMunchState *create_max_munch_state(void);
InstructionTile *add_tile_left_subtree(InstructionTile *tile,
                                       InstructionTile *subtree);
InstructionTile *add_tile_right_subtree(InstructionTile *tile,
                                        InstructionTile *subtree);
MaxMunchState *add_munch_state_tile(MaxMunchState *state,
                                    InstructionTile *tile);
MaxMunchState *add_munch_state_inst(MaxMunchState *state,
                                    MachineInstruction *minst);
void free_instruction_tile(InstructionTile *tile);
void free_machine_register(MachineRegister *reg);
void free_maximal_munch_state(MaxMunchState *state);

// + K: IEEE-745 Interface +

typedef struct IEEE745_Float32 Float32;
typedef struct IEEE745_Float64 Float64;
typedef bool sign_t;

Float32 create_float32(uint32_t fraction, uint8_t exponent, sign_t sign);
Float64 create_float64(uint64_t fraction, uint16_t exponent, sign_t sign);
Float32 f32_addition(Float32 a, Float32 b);
Float32 f32_subtraction(Float32 a, Float32 b);
Float32 f32_multiplication(Float32 a, Float32 b);
Float32 f32_division(Float32 a, Float32 b);
Float32 f32_exponentiation(Float32 a, Float32 b);
Float64 f64_addition(Float64 a, Float64 b);
Float64 f64_subtraction(Float64 a, Float64 b);
Float64 f64_multiplication(Float64 a, Float64 b);
Float64 f64_division(Float64 a, Float64 b);
Float64 f64_exponentiation(Float64 a, Float64 b);

#define IEEE745_SIGN_NEGATIVE 0
#define IEEE745_SIGN_POSITIVE 1

#define FLOAT32_FRACTION_MASK 0x800000
#define FLOAT32_BIAS 127

#define FLOAT64_FRACTION_MASK 0x10000000000000
#define FLOAT64_BIAS 1023

// + L: The Typing System +

typedef enum TypeKind TypeKind;
typedef enum TypeStaticity TypeStaticity;
typedef enum TypeStrength TypeStrength;
typedef enum BaseWordType BaseWordType;
typedef struct SingletonType SingletonType;
typedef struct TypeField TypeField;
typedef struct TypeEnumeration TypeEnumeration;
typedef struct TypeVirtualTable TypeVirtualTable;
typedef size_t typesize_t;
typedef int typeid_t;

SingletonType *create_singleton_type(TypeKind kind, TypeStaticity staticity,
                                     TypeStrength strength);
SingletonType *add_singleton_type_fields(SingletonType *singleton);
SingletonType *add_singleton_type_enumeration_fields(SingletonType *singleton);
SingletonType *add_singleton_type_vtable(SingletonType *singleton);
TypeFields *create_type_fields(void);
TypeEnumerationFields *create_type_enumeration_fields(void);
TypeVirtualTable *create_type_vtable(void);
SingletonType *add_singleton_field(SingletonType *singleton,
                                   SingletonType *field);
SingletonType *add_singleton_enumeration_field(SingletonType *singleton,
                                               TypeEnumeration *enumeration);
SingletonType *add_singleton_vtable_method(SingletonType *singleton,
                                           TypeMethod *meth);

// + M: Constant Folding +

Variable *add_variables(Variable *op1, Variable *op2);
Variable *subtract_variables(Variable *op1, Variable *op2);
Variable *multiply_variables(Variable *op1, Variable *op2);
Variable *divide_variables(Variable *op1, Variable *op2);
Variable *modulo_variables(Variable *op1, Variable *op2);
Variable *exponentiate_variables(Variable *op1, Variable *op2);
Variable *bitwise_and_variables(Variable *op1, Variable *op2);
Variable *bitwise_or_variables(Variable *op1, Variable *op2);
Variable *bitwise_xor_variables(Variable *op1, Variable *op2);
Variable *bitwise_shr_variables(Variable *op1, Variable *op2);
Variable *bitwise_shl_variables(Variable *op1, Variable *op2);
bool attempt_constant_folding(Instruction *inst);

// === Some Systems Macros ====

#define FREE_AND_NULLIFY(MEM)                                                  \
  do {                                                                         \
    free(*MEM);                                                                \
    *MEM = NULL;                                                               \
  } while (0)

#ifdef __unix__
#define SYSTEM_NEWLINE "\n"
#else
#define SYSTEM_NEWLINE "\r\n"
#endif

#endif
