#include "generator.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

int create_register(ir_operand_node_t *result) {
    result = malloc(sizeof(ir_operand_node_t));
    if (result == NULL) {
        fprintf(stderr, "generate: failed to allocate memory\n");
        return -1;
    }

    result->type = IR_OPERAND_REGISTER;
    return 0;
}

int immediate_from_expression(ast_expression_node_t *expression, ir_operand_node_t **result) {
    *result = malloc(sizeof(ir_operand_node_t));
    if (result == NULL) {
        fprintf(stderr, "generate: failed to allocate memory\n");
        return -1;
    }

    (*result)->type = IR_OPERAND_IMMEDIATE;
    (*result)->data.immediate.value = expression->data.constant_expression.value;
    return 0;
}

int instructions_from_statement(ast_statement_node_t *statement, list_t *result) {
    ir_instruction_node_t *mov = malloc(sizeof(ir_instruction_node_t));
    mov->type = IR_INSTRUCTION_MOV;
    list_append(result, mov);

    if (immediate_from_expression(statement->data.return_statement.expression, &mov->data.mov.left) != 0) {
        fprintf(stderr, "Failed to convert expression to immediate.\n");
        return -1;
    }

    if (create_register(mov->data.mov.right) != 0) {
        fprintf(stderr, "Failed to create register.\n");
        return -1;
    }

    ir_instruction_node_t *ret = malloc(sizeof(ir_instruction_node_t));
    ret->type = IR_INSTRUCTION_RET;
    list_append(result, ret);
    return 0;
}

int generate(ast_program_node_t *ast_program, ir_program_node_t *result) {
    result->functions = list_create(128);

    ir_function_node_t *function_node = malloc(sizeof(ir_function_node_t));
    if (function_node == NULL) {
        fprintf(stderr, "generate: failed to allocate memory\n");
        return -1;
    }
    list_append(&result->functions, function_node);

    ast_definition_node_t *ast_definition = list_get(&ast_program->definitions, 0);
    function_node->name = ast_definition->data.function_definition.name;

    function_node->instructions = list_create(128);
    if (instructions_from_statement(ast_definition->data.function_definition.body, &function_node->instructions) != 0) {
        fprintf(stderr, "Failed to convert statement to instructions.\n");
        return -1;
    }

    return 0;
}
