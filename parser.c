#include "parser.h"
#include "lexer.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

#define EXPECT(expected_type, error_format)                                                                   \
    if (*current_token_idx == lexems->count) {                                                                \
        fprintf(stderr, "Found end of tokens list [index %d]: ", *current_token_idx);                         \
        fprintf(stderr, error_format);                                                                        \
        fprintf(stderr, "\n");                                                                                \
        return -1;                                                                                            \
    }                                                                                                         \
    if (((token_t *)list_get(lexems, *current_token_idx))->type != expected_type) {                           \
        fprintf(stderr, "Parsing Error [index %d]: ", *current_token_idx);                                    \
        fprintf(stderr, error_format);                                                                        \
        fprintf(stderr, " (found '%s' instead)\n", ((token_t *)list_get(lexems, *current_token_idx))->value); \
        return -1;                                                                                            \
    }                                                                                                         \
    (*current_token_idx)++;

#define RETURN_EXPECTED(result_token, expected_type, error_format)                                            \
    if (*current_token_idx == lexems->count) {                                                                \
        fprintf(stderr, "Found end of tokens at index %d.\n", *current_token_idx);                            \
        return -1;                                                                                            \
    }                                                                                                         \
    result_token = ((token_t *)list_get(lexems, *current_token_idx));                                         \
    if (result_token->type != expected_type) {                                                                \
        fprintf(stderr, "Parsing Error [index %d]: ", *current_token_idx);                                    \
        fprintf(stderr, error_format);                                                                        \
        fprintf(stderr, " (found '%s' instead)\n", ((token_t *)list_get(lexems, *current_token_idx))->value); \
        return -1;                                                                                            \
    }                                                                                                         \
    (*current_token_idx)++;

int parse_expression(list_t *lexems, int *current_token_idx, ast_expression_node_t *result) {
    // for now we assume that our only expression is constant
    token_t *constant_token;
    result->type = AST_EXPRESSION_CONSTANT;
    RETURN_EXPECTED(constant_token, TOKEN_CONSTANT, "expected constant");

    result->data.constant_expression.value = atoi(constant_token->value);
    return 0;
}

int parse_statement(list_t *lexems, int *current_token_idx, ast_statement_node_t *result) {
    // for now we assume that our only statement is "return ...;"
    result->type = AST_STATEMENT_RETURN;
    EXPECT(TOKEN_RETURN_KEYWORD, "expected return");

    result->data.return_statement.expression = malloc(sizeof(ast_expression_node_t));
    int epxression_result = parse_expression(lexems, current_token_idx, result->data.return_statement.expression);
    if (epxression_result == -1) {
        fprintf(stderr, "Failed to parse statement.\n");
        return -1;
    }

    EXPECT(TOKEN_SEMICOLON, "expected ;");
    return 0;
}

int parse_definition(list_t *lexems, int *current_token_idx, ast_definition_node_t *result) {
    // for now we assume that our only definition is "int main(void) { ... }"
    result->type = AST_DEFINITION_FUNCTION;
    EXPECT(TOKEN_INT_KEYWORD, "expected int");

    token_t *identifier_token;
    RETURN_EXPECTED(identifier_token, TOKEN_IDENTIFIER, "expected function name")
    result->data.function_definition.name = identifier_token->value;

    EXPECT(TOKEN_OPEN_PARENTHESIS, "expected (");
    EXPECT(TOKEN_VOID_KEYWORD, "expected void");
    EXPECT(TOKEN_CLOSE_PARENTHESIS, "expected )");
    EXPECT(TOKEN_OPEN_BRACE, "expected {");

    result->data.function_definition.body = malloc(sizeof(ast_statement_node_t));
    int statement_result = parse_statement(lexems, current_token_idx, result->data.function_definition.body);
    if (statement_result == -1) {
        fprintf(stderr, "Failed to parse statement.\n");
        return -1;
    }

    EXPECT(TOKEN_CLOSE_BRACE, "expected }");
    return 0;
}

int parse(list_t *lexems, ast_program_node_t *result) {
    // for now program equals function, so just pass everything to parse_definition
    int current_token_idx = 0;
    result->definitions = list_create(128);

    ast_definition_node_t *function_node = malloc(sizeof(ast_definition_node_t));
    if (function_node == NULL) {
        fprintf(stderr, "parse: failed to allocate memory\n");
        return -1;
    }

    list_append(&result->definitions, function_node);
    int tokens_read = parse_definition(lexems, &current_token_idx, ((ast_definition_node_t *)list_get(&result->definitions, 0)));
    if (tokens_read == -1) {
        return -1;
    }

    if (current_token_idx < lexems->count) {
        fprintf(stderr, "Found junk after main function: %s.\n", ((token_t *)list_get(lexems, current_token_idx))->value);
        return -1;
    }

    return 0;
}
