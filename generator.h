#ifndef GENERATOR_H
#define GENERATOR_H

#include "list.h"
#include "parser.h"

// forward declarations to use later
typedef struct ir_function_node ir_function_node_t;
typedef struct ir_instruction_node ir_instruction_node_t;
typedef struct ir_operand_node ir_operand_node_t;

typedef struct {
    list_t functions;
} ir_program_node_t;

// functions
struct ir_function_node {
    char *name;
    list_t instructions;
};

// instructions
typedef enum {
    IR_INSTRUCTION_MOV,
    IR_INSTRUCTION_RET,
} ir_instruction_type_t;

struct ir_instruction_node {
    ir_instruction_type_t type;
    union {
        struct {
            ir_operand_node_t *left;
            ir_operand_node_t *right;
        } mov;
    } data;
};

// operands
typedef enum {
    IR_OPERAND_IMMEDIATE,
    IR_OPERAND_REGISTER,
} ir_operand_type_t;

struct ir_operand_node {
    ir_operand_type_t type;
    union {
        struct {
            int value;
        } immediate;
    } data;
};

int generate(ast_program_node_t *ast_program, ir_program_node_t *result);

#endif
