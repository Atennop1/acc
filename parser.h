#ifndef PARSER_H
#define PARSER_H

#include "list.h"

// forward declarations for all node types
typedef struct ast_definition_node ast_definition_node_t;
typedef struct ast_statement_node ast_statement_node_t;
typedef struct ast_expression_node ast_expression_node_t;

// program (root node)
typedef struct {
    list_t definitions;
} ast_program_node_t;

// definitions
typedef enum {
    AST_DEFINITION_FUNCTION,
} ast_definition_type_t;

struct ast_definition_node {
    ast_definition_type_t type;
    union {
        struct {
            char *name;
            ast_statement_node_t *body;
        } function_definition;
    } data;
};

// statements
typedef enum {
    AST_STATEMENT_RETURN,
} ast_statement_type_t;

struct ast_statement_node {
    ast_statement_type_t type;
    union {
        struct {
            ast_expression_node_t *expression;
        } return_statement;
    } data;
};

// expressions
typedef enum {
    AST_EXPRESSION_CONSTANT,
} ast_expression_type_t;

struct ast_expression_node {
    ast_expression_type_t type;
    union {
        struct {
            int value;
        } constant_expression;
    } data;
};

int parse(list_t *lexems, ast_program_node_t *result);

#endif
