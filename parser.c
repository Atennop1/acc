#include "parser.h"
#include "lexer.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

token_t *expect(list_t *lexems, int *current_token_idx, token_type_t type) {
    token_t *token = (token_t *)list_get(lexems, *current_token_idx);
    if (token->type != type) { return NULL; }

    (*current_token_idx)++;
    return token;
}

int parse_expression(list_t *lexems, int *current_token_idx, expression_node_t *result) {
    // for now we assume that our only expression is constant
    result->type = EXPRESSION_CONSTANT;

    token_t *constant_token = expect(lexems, current_token_idx, TOKEN_CONSTANT);
    if (constant_token == NULL) {
        fprintf(stderr, "Function definition parsing failed: expected constant but found %s.\n", ((token_t *)list_get(lexems, *current_token_idx))->value);
        return -1;
    }

    result->data.constant_expression.value = atoi(constant_token->value);
    return 0;
}

int parse_statement(list_t *lexems, int *current_token_idx, statement_node_t *result) {
    // for now we assume that our only statement is "return ...;"
    result->type = STATEMENT_RETURN;

    token_t *return_token = expect(lexems, current_token_idx, TOKEN_RETURN_KEYWORD);
    if (return_token == NULL) {
        fprintf(stderr, "Function definition parsing failed: expected return but found %s.\n", ((token_t *)list_get(lexems, *current_token_idx))->value);
        return -1;
    }

    result->data.return_statement.expression = malloc(sizeof(expression_node_t));
    int epxression_result = parse_expression(lexems, current_token_idx, result->data.return_statement.expression);
    if (epxression_result == -1) {
        fprintf(stderr, "Failed to parse statement.\n");
        return -1;
    }

    token_t *semicolon_token = expect(lexems, current_token_idx, TOKEN_SEMICOLON);
    if (semicolon_token == NULL) {
        fprintf(stderr, "Function definition parsing failed: expected ; but found %s.\n", ((token_t *)list_get(lexems, *current_token_idx))->value);
        return -1;
    }

    return 0;
}

int parse_definition(list_t *lexems, int *current_token_idx, definition_node_t *result) {
    // for now we assume that our only definition is "int main(void) { ... }"
    result->type = DEFINITION_FUNCTION;

    token_t *int_token = expect(lexems, current_token_idx, TOKEN_INT_KEYWORD);
    if (int_token == NULL) {
        fprintf(stderr, "Function definition parsing failed: expected int but found %s.\n", ((token_t *)list_get(lexems, *current_token_idx))->value);
        return -1;
    }

    token_t *identifier_token = expect(lexems, current_token_idx, TOKEN_IDENTIFIER);
    if (identifier_token == NULL) {
        fprintf(stderr, "Function definition parsing failed: expected function name but found %s.\n", ((token_t *)list_get(lexems, *current_token_idx))->value);
        return -1;
    }
    result->data.function_definition.name = identifier_token->value;

    token_t *open_parenthesis_token = expect(lexems, current_token_idx, TOKEN_OPEN_PARENTHESIS);
    if (open_parenthesis_token == NULL) {
        fprintf(stderr, "Function definition parsing failed: expected ( but found %s.\n", ((token_t *)list_get(lexems, *current_token_idx))->value);
        return -1;
    }

    token_t *void_token = expect(lexems, current_token_idx, TOKEN_VOID_KEYWORD);
    if (void_token == NULL) {
        fprintf(stderr, "Function definition parsing failed: expected void but found %s.\n", ((token_t *)list_get(lexems, *current_token_idx))->value);
        return -1;
    }

    token_t *closed_parenthesis_token = expect(lexems, current_token_idx, TOKEN_CLOSE_PARENTHESIS);
    if (closed_parenthesis_token == NULL) {
        fprintf(stderr, "Function definition parsing failed: expected ) but found %s.\n", ((token_t *)list_get(lexems, *current_token_idx))->value);
        return -1;
    }

    token_t *open_brace_token = expect(lexems, current_token_idx, TOKEN_OPEN_BRACE);
    if (open_brace_token == NULL) {
        fprintf(stderr, "Function definition parsing failed: expected { but found %s.\n", ((token_t *)list_get(lexems, *current_token_idx))->value);
        return -1;
    }

    result->data.function_definition.body = malloc(sizeof(statement_node_t));
    int statement_result = parse_statement(lexems, current_token_idx, result->data.function_definition.body);
    if (statement_result == -1) {
        fprintf(stderr, "Failed to parse statement.\n");
        return -1;
    }

    token_t *close_brace_token = expect(lexems, current_token_idx, TOKEN_CLOSE_BRACE);
    if (close_brace_token == NULL) {
        fprintf(stderr, "Function definition parsing failed: expected } but found %s.\n", ((token_t *)list_get(lexems, *current_token_idx))->value);
        return -1;
    }

    return 0;
}

int parse(list_t *lexems, program_node_t *result) {
    // for now program equals function, so just pass everything to parse_definition
    int current_token_idx = 0;
    result->definitions = list_create(128);

    definition_node_t *function_node = malloc(sizeof(definition_node_t));
    if (function_node == NULL) {
        fprintf(stderr, "parse: failed to allocate memory\n");
        return -1;
    }

    list_append(&result->definitions, function_node);
    int tokens_read = parse_definition(lexems, &current_token_idx, ((definition_node_t *)list_get(&result->definitions, 0)));
    if (tokens_read == -1) {
        return -1;
    }

    return 0;
}
