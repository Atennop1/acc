#include "lexer.h"
#include "list.h"
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    token_type_t type;
    char *pattern;
    regex_t compiled_regex;
} token_context_t;

token_context_t patterns[] = {
    {TOKEN_IDENTIFIER, "[a-zA-Z_]\\w*\\b"},
    {TOKEN_CONSTANT, "[0-9]+\\b"},
    {TOKEN_INT_KEYWORD, "int\\b"},
    {TOKEN_VOID_KEYWORD, "void\\b"},
    {TOKEN_RETURN_KEYWORD, "return\\b"},
    {TOKEN_OPEN_PARENTHESIS, "\\("},
    {TOKEN_CLOSE_PARENTHESIS, "\\)"},
    {TOKEN_OPEN_BRACE, "\\{"},
    {TOKEN_CLOSE_BRACE, "\\}"},
    {TOKEN_SEMICOLON, ";"},
};

const int TOKENS_COUNT = sizeof(patterns) / sizeof(patterns[0]);

char *read_file(const char *file_path) {
    FILE *file = fopen(file_path, "rb");

    if (file == NULL) {
        fprintf(stderr, "Failed to open file: %s.\n", file_path);
        return NULL;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fprintf(stderr, "Failed to find the end of file: %s.\n", file_path);
        fclose(file);
        return NULL;
    }

    long length = ftell(file);
    if (length == -1) {
        fprintf(stderr, "Failed to determine length of file: %s.\n", file_path);
        fclose(file);
        return NULL;
    }

    if (fseek(file, 0, SEEK_SET) != 0) {
        fprintf(stderr, "Failed to find the beginning of file: %s.\n", file_path);
        fclose(file);
        return NULL;
    }

    char *buffer = (char *)malloc(length + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Error allocating memory: %ld.\n", length + 1);
        fclose(file);
        return NULL;
    }

    size_t read_size = fread(buffer, 1, length, file);
    if (read_size != length) {
        fprintf(stderr, "Error reading file into buffer: read %zu bytes, expected %ld bytes.\n", read_size, length);
        free(buffer);
        fclose(file);
        return NULL;
    }

    buffer[length] = '\0';
    fclose(file);
    return buffer;
}

int lex(const char *source_file_path, list_t *output) {
    char *input = read_file(source_file_path);
    if (input == NULL) { return -1; }

    // compiling regular expressions
    char err_buffer[256];
    for (int i = 0; i < TOKENS_COUNT; i++) {
        int reti = regcomp(&patterns[i].compiled_regex, patterns[i].pattern, REG_EXTENDED);

        if (reti != 0) {
            regerror(reti, &patterns[i].compiled_regex, err_buffer, sizeof(err_buffer));
            fprintf(stderr, "Error compiling regex %s: %s.\n", patterns[i].pattern, err_buffer);
            return -1;
        }
    }

    // iterating through input until the end
    char *input_ptr = input;
    while (*input_ptr != '\0') {
        if (*input_ptr == ' ' || *input_ptr == '\n' || *input_ptr == '\t') {
            input_ptr++;
            continue;
        }

        regmatch_t match;
        int max_token_length = -1;
        int best_token_index = -1;

        // searching for matching of some token at the beginning of string
        for (int i = 0; i < TOKENS_COUNT; i++) {
            switch (regexec(&patterns[i].compiled_regex, input_ptr, 1, &match, 0)) {
            case 0:
                break;
            case REG_NOMATCH:
                continue;
            default:
                fprintf(stderr, "Failed matching regex for %s\n", patterns[i].pattern);
                return -1;
            }

            if (match.rm_so == 0 && match.rm_eo - match.rm_so > max_token_length) {
                max_token_length = match.rm_eo - match.rm_so;
                best_token_index = i;
            }
        }

        // throwing an error if token was not found
        if (max_token_length == -1) {
            fprintf(stderr, "Faced unknown token at the beggining of string: %s\n", input_ptr);
            return -1;
        }

        token_t *new_token = malloc(sizeof(token_t));
        new_token->type = patterns[best_token_index].type;
        new_token->value = malloc(max_token_length + 1);
        strncpy(new_token->value, input_ptr, max_token_length);
        new_token->value[max_token_length] = '\0';

        list_append(output, new_token);
        input_ptr += max_token_length;
    }

    // freeing up
    for (int i = 0; i < TOKENS_COUNT; i++) { regfree(&patterns[i].compiled_regex); }
    free(input);
    return 0;
}
