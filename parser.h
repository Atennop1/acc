#ifndef PARSER_H
#define PARSER_H

#include "list.h"

// forward declarations for all node types
typedef struct definition_node definition_node_t;
typedef struct statement_node statement_node_t;
typedef struct expression_node expression_node_t;

// program (root node)
typedef struct {
    list_t definitions;
} program_node_t;

// definitions
typedef enum {
    DEFINITION_FUNCTION,
} definition_type_t;

struct definition_node {
    definition_type_t type;
    union {
        struct {
            char *name;
            statement_node_t *body;
        } function_definition;
    } data;
};

// statements
typedef enum {
    STATEMENT_RETURN,
} statement_type_t;

struct statement_node {
    statement_type_t type;
    union {
        struct {
            expression_node_t *expression;
        } return_statement;
    } data;
};

// expressions
typedef enum {
    EXPRESSION_CONSTANT,
} expression_type_t;

struct expression_node {
    expression_type_t type;
    union {
        struct {
            int value;
        } constant_expression;
    } data;
};

int parse(list_t *lexems, program_node_t *result);

#endif
