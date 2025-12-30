#ifndef LEXER_H
#define LEXER_H

#include "list.h"

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_CONSTANT,
    TOKEN_INT_KEYWORD,
    TOKEN_VOID_KEYWORD,
    TOKEN_RETURN_KEYWORD,
    TOKEN_OPEN_PARENTHESIS,
    TOKEN_CLOSE_PARENTHESIS,
    TOKEN_OPEN_BRACE,
    TOKEN_CLOSE_BRACE,
    TOKEN_SEMICOLON,
} token_type_t;

typedef struct {
    token_type_t type;
    char *value;
} token_t;

int lex(const char *source_file_path, list_t *ouput);

#endif
