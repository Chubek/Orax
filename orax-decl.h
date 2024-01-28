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
typedef int vert_t;

DAGNode *create_dag_node(Instruction *inst, vert_t dest);
DAGGraph *create_dag_graph(vert_t vertices);
void add_dag_edge(DAGGraph **graph, vert_t src, vert_t dst, Instruction *inst);

void free_dag_node(DAGNode *root);
void free_dag_graph(DAGGraph *graph);

// + D: The Instructions +

typedef enum InstructionName InstructionName; // Defined in `orax-enums.h`
typedef enum OperandType OperandType;         // Defined in `orax-enums.h`
typedef enum OperandType ResultType;          // Defined in `orax-enums.h`
typedef struct SSAInfo SSAInfo;
typedef struct Instruction Instruction;
typedef struct Operand Operand;
typedef struct operand Result;
typedef int instid_t;
typedef int ophash_t;
typedef int ssaversion_t;

Instruction *create_instruction(InstructionName type, instid_t instruction_id);
Instruction *add_inst_operand(Instruction *inst, Operand *operand);
Instruction *add_inst_result(Instruction *inst, Result *result);
Operand *create_operand(opid_t id, OperandType type, void *value);
Result *create_result(ResultType type, void *value);
Operand *duplicate_operand(Operand *op);
void free_operand(Operand *op);
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
void ssa_reversion_operands(ControlFlowGraph *cfg);
void analyze_liveness(ControlFlowGraph *graph);
bool dominates(TraceBlock *dominator, TraceBlock *block);
bool operand_defined_in_block(Operand *operand, TraceBlock *block);
bool is_phi_present_for_operand(TraceBlock *block, Operand *operand);
Instruction *create_instruction(TraceBlock *block, Operand *operand);
Operand *get_operand_from_block(Operand *operand, TraceBlock *block);
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

typedef struct Operand LifeObject;
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

// + J: S-Expression Parser +

typedef enum SExpressionType SExpressionType; // Defined in `orax-enums.h`
typedef struct SExpression SExpression;
typedef struct SExpressionList SExpressionList;
typedef struct SExpressionSynObj SExpressionSynObj;
typedef struct SExpressionMacro SExpressionMacro;

bool is_valid_atom_punct(char c);
SExpression *create_sexp(SExpressionType type);
SExpressionList *create_sexp_list(void);
SExpressionList *add_sexpls_node(SExpressionList *sexpls, SExpression *sexp);
SExpression *parse_sexp_atom(FILE *input_file);
SExpressionList *parse_sexp_list(FILE *input_file);
SExpressionSynObj *create_sexp_synobj(char *name);
SExpressionSynObj *add_synobj_parameter(SExpressionSynObj *synobj,
                                        char *parameter);
SExpressionSynObj *add_synobj_argument(SExpressionSynObj *synobj,
                                       SExpressionSynObj *argument);
SExpressionSynObj *add_synobj_expression(SExpressionSynObj *synobj,
                                         SExpression *expr);
void walk_sexp_list(SExpressionList *sexpls);
void print_sexp(SExpression *sexp);
void free_sexp_list(SExpressionList *sexpls);
void free_sexp(SExpression *sexp);
void free_sexp_synobj(SExpressionSynObj *synobj);

#define SEXP_BUFF_MAX 2048

// + K: IEEE-745 Interface +

typedef struct Float32 Float32;
typedef struct Float64 Float64;

Float32 newFloat32(uint32_t fraction, uint8_t exponent, uint8_t sign);
Float64 newFloat64(uint64_t fraction, uint16_t exponent, uint8_t sign);
Float32 f32Addition(Float32 a, Float32 b);
Float32 f32Subtraction(Float32 a, Float32 b);
Float32 f32Multiplication(Float32 a, Float32 b);
Float32 f32Division(Float32 a, Float32 b);
Float32 f32Exponentiation(Float32 a, Float32 b);
Float64 f64Addition(Float64 a, Float64 b);
Float64 f64Subtraction(Float64 a, Float64 b);
Float64 f64Multiplication(Float64 a, Float64 b);
Float64 f64Division(Float64 a, Float64 b);
Float64 f64Exponentiation(Float64 a, Float64 b);

#define FLOAT32_FRACTION_MASK 0x800000
#define FLOAT32_BIAS 127

#define FLOAT64_FRACTION_MASK 0x10000000000000
#define FLOAT64_BIAS 1023

// + L: LALR Parser Generator +

typedef enum SemanticItemType SemanticItemType; // Defined in `orax-enums.h`
typedef struct SemanicItem SemanticItem;
typedef struct Production Production;
typedef struct LALRRule LALRRule;
typedef struct SymbtabNode LALRSymtab;
typedef struct LR0Item LR0Item;
typedef struct LR0State LR0State;

SemanticItem *create_semantic_item(SemanticItemType type, char *value);
Production *create_production(void);
Production *add_production_semitem(Production *prod, SemanticItem *semitem);
LALRRule *create_yacc_rule(char *name, char *production, char *semantic_action);
LR0Item *create_lr0_item(LALRRule *rule);
LR0State *create_lr0_state(void);
LR0State *lr0_state_add_item(LR0State *state, LR0Item *item);

#define DOT_INDEX_INIT -1

// + M: The Lexican Scanner Generator +

typedef struct NFAState NFAState;
typedef struct DFAState DFAState;
typedef int nfaid_t;
typedef int dfaid_t;

NFAState *create_nfa_state(nfaid_t id, bool is_accepting);
DFAState *create_dfa_state(dfaid_t id, bool is_accepting);
NFAState *add_nfa_epstrans(NFAState *state, NFAState *eps);
void add_nfa_trans(NFAState *state, uint32_t from, uint32_t to);
void add_dfa_trans(DFAState *state, uint32_t from, uint16_t to);
bool state_in_set(NFAState **set, size_t size, NFAState *state);
DFAState *convert_nfa_to_dfa(NFAState *nfa);

#define MAX_TRANSITIONS 65536
#define EPSILON 65537

// + N: The LL Parser Generator +

typedef enum PEGSymbol PEGSymbol	// Defined in `orax-enums.h`
typedef struct PEGExpression PEGExpression;
typedef union PEGMetaExpression PEGMetaExpression;


#define PEG_CHAR_UNASSIGNED 65537
#define MAX_PEG_SUBEXPRESSION 2
#define PEG_SUBEXP_INDEX_SEQUENCE 0
#define PEG_SUBEXP_INDEX_CHOICE 1
#define PEG_SUBEXP_INDEX_GROUP 2





#endif
