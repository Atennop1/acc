#include "emitter.h"
#include "generator.h"
#include "lexer.h"
#include "list.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠛⠀⠙⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠗⠀⠀⣀⣄⠀⢿⣿⣿⣿⠟⠁⢠⡆⠉⠙⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠀⠀⣴⣿⡟⠀⠘⣿⣿⠋⠀⠀⠀⢠⣶⡀⠈⢻⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⠀⠀⢠⣿⠛⣶⠀⠀⣿⡟⠀⠀⠀⢠⣿⣿⡇⠀⠠⣽⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠅⠀⠀⣿⠏⠀⣿⠀⠀⣿⠁⠀⠀⢠⣿⠟⢻⡇⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡏⠀⠀⣼⣿⠀⢰⡟⠀⠀⠛⠀⠀⠀⣾⡇⠀⢸⡇⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠿⠃⠀⠈⠀⢀⠀⣀⣀⠀⠘⠟⠀⠀⡾⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠃⠀⠀⠀⢀⠂⠀⠈⠉⢴⣽⣿⠵⣿⡶⣂⣄⡀⠀⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⡟⠡⠆⢀⠀⠀⠀⠀⠄⠀⠈⠘⠏⢿⣶⣿⡿⢟⣱⣖⠒⠀⠘⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⡟⣻⠏⣠⠄⠀⢀⡀⠀⠀⠀⠀⠈⠀⠀⠀⢸⣿⢦⠄⠙⣿⡇⠩⣭⣅⠈⢿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣟⣼⡇⠈⢀⣴⠆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠁⠀⢀⠀⠈⠰⣶⡤⠿⠃⢸⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⡟⠉⢠⡶⠋⠀⠀⠀⠀⠀⠀⠀⢀⣤⣤⣴⣶⣤⣄⡀⠀⠀⠂⠀⠀⠀⠀⠀⠀⠈⣿⣿⣿⣿⣿
// ⣿⣿⣿⡏⢀⡠⠀⠀⠀⠀⠀⠀⠀⢀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣶⣶⣦⣄⠀⠀⠂⠀⠈⣿⣿⣿⣿
// ⣿⣿⣿⢃⠈⠀⢠⠀⠀⠀⠀⠀⢠⣿⣿⣿⠿⣩⣏⡙⣛⣛⣿⣿⣿⣿⣿⣿⣿⡿⢇⠀⠀⠄⠀⠘⣿⣿⣿
// ⣿⣿⣿⡎⠀⠀⠀⠀⠀⠀⠀⠠⣿⣿⣿⡟⣰⣿⠁⢀⠈⢿⣿⣿⣿⣿⢁⣴⠖⢲⣾⡇⠀⠀⠄⠀⣿⣿⣿
// ⣿⣿⣿⢀⠀⠀⠀⠀⠀⠀⠀⠀⣏⢿⣿⡇⣿⡇⠀⠀⠀⣼⣿⣿⣿⡇⣼⡏⠀⠀⣿⡇⠀⠀⠀⠀⣻⣿⣿
// ⣿⣿⣿⣇⠀⠀⠀⠀⠀⠀⠀⠀⢸⣄⠻⣷⡘⣷⣀⣀⣴⣿⡟⠉⠛⠓⣿⡇⠀⢰⣿⡇⠀⠀⠀⣼⣿⣿⣿
// ⣿⣿⣿⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀⠙⢷⣌⠻⢿⣿⣿⣿⣿⣿⣦⣶⣿⣾⣧⣤⡾⠏⠀⠀⠀⠀⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠻⠶⢌⣉⣛⠛⠿⠿⠿⠿⠿⠛⠉⠀⠀⠀⠀⣰⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣶⣄⠀⠀⠀⠀⠲⠀⠀⠀⠀⠀⠀⠉⠉⠉⠀⠀⠀⠈⠁⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠛⠻⠿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⡏⠛⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡘⡻⣿⣿
// ⣿⣿⣿⣿⣿⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⢨⡛⡛⣁⣿
// ⣿⣿⣿⣿⡿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠂⠀⠀⠀⠀⠀⠀⠀⠀⣠⣿⣿⣿
// ⣿⣿⣿⣿⠇⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣤⣄⣠⣴⣾⣿⣿⣿⣿
// ⣿⣿⣿⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⡿⠋⠠⣾⡇⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿
// ⡟⢁⣠⣤⣦⣌⡃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣏⡹⢿⣿⣿⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⡿⠿⢶⡬⠙⠟⠋⣁⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣏⠛⠚⠃⠀⠀⢰⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣻⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣧⣆⣤⣄⣤⣼⠁⠀⠀⢀⠀⠀⠀⠀⠀⠀⠀⠠⠒⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠻⠿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣧⣤⣴⣴⣦⠀⣄⠀⠀⠀⠀⠀⠀⠀⠀⢀⡀⠀⠐⠚⠛⠓⠂⢀⡄⠀⢰⢽⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣰⣤⣄⣀⢀⠀⢠⡅⠀⠀⢀⣤⣤⡼⠧⣤⣤⣠⣤⣤⣄⡀⡀⣰⣦⣣⠈⡡⣽
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣹⣿⣿⣿⣾⣷⣾⡗⣒⠶⣿⣿⣿⡷⣾⣿⣿⣿⣿⣿⣿⢿⣿⣿⣿⣿⣿⣿⣿

char *get_basename_without_ext(const char *path) {
    char *basename = strdup(path);
    if (basename == NULL) { return NULL; }

    char *last_dot = strrchr(basename, '.');
    if (last_dot != NULL && last_dot != basename) { *last_dot = '\0'; }

    return basename;
}

int main(int argc, char **argv) {
    int until_lexer = 0;
    int until_parser = 0;
    int until_codegen = 0;

    int do_emit = 0;
    char *source_file_path = NULL;
    char *output_file_path = NULL;

    // parsing command line arguments
    int current_argument_idx = 1;
    while (current_argument_idx < argc) {
        char *argument = argv[current_argument_idx];

        if (strcmp(argument, "--lex") == 0 || strcmp(argument, "-l") == 0) {
            until_lexer = 1;
            current_argument_idx++;
            continue;
        }

        if (strcmp(argument, "--parse") == 0 || strcmp(argument, "-p") == 0) {
            until_parser = 1;
            current_argument_idx++;
            continue;
        }

        if (strcmp(argument, "--codegen") == 0 || strcmp(argument, "-c") == 0) {
            until_codegen = 1;
            current_argument_idx++;
            continue;
        }

        if (strcmp(argument, "-S") == 0) {
            do_emit = 1;
            current_argument_idx++;
            continue;
        }

        if (strcmp(argument, "-o") == 0) {
            output_file_path = argv[current_argument_idx + 1];
            current_argument_idx += 2;
            continue;
        }

        if (argument[0] == '-') {
            fprintf(stderr, "Unknown flag: %s.\n", argument);
            return 1;
        }

        if (source_file_path != NULL) {
            fprintf(stderr, "Only one source file is allowed, have at least 2: %s and %s.\n", source_file_path, argument);
            return 1;
        }

        source_file_path = argument;
        current_argument_idx++;
    }

    if (source_file_path == NULL) {
        fprintf(stderr, "No source file provided.\n");
        return 1;
    }

    if (strlen(source_file_path) > 256) {
        fprintf(stderr, "Source file path is too long.\n");
        return 1;
    }

    if ((until_lexer + until_parser + until_codegen) > 1) {
        fprintf(stderr, "Ambigous: can stop either after lexer, parser, code generator, or emitter.\n");
        return 1;
    }

    char command[512];
    int status_code = 0;

    // preprocessing source file
    snprintf(command, sizeof(command), "gcc -E -P %s -o preprocessed.i", source_file_path);
    status_code = system(command);
    if (status_code != 0) {
        fprintf(stderr, "Something went wrong on the preprocessing step: %d.\n", status_code);
        return 1;
    }

    printf("Preprocessed.\n");

    // tokenizing preprocessed file
    list_t tokens_list = list_create(128);
    if (lex("preprocessed.i", &tokens_list) != 0) {
        fprintf(stderr, "Lexing failed.\n");
        return 1;
    }

    status_code = system("rm preprocessed.i");
    if (status_code != 0) {
        fprintf(stderr, "Something went wrong on %s deletion: %d.\n", "preprocessed.i", status_code);
        return 1;
    }

    printf("Lexed.\n");
    if (until_lexer) {
        printf("Stopping here because --lex was provided.\n");
        return 0;
    }

    // parsing tokens
    ast_program_node_t *ast_program_node = malloc(sizeof(ast_program_node_t));
    if (ast_program_node == NULL) {
        fprintf(stderr, "main: Failed to allocate memory.\n");
        return 1;
    }

    if (parse(&tokens_list, ast_program_node) != 0) {
        fprintf(stderr, "Parsing failed.\n");
        return 1;
    }

    printf("Parsed.\n");
    if (until_parser) {
        printf("Stopping here because --parse was provided.\n");
        return 0;
    }

    // generating intermediate representation
    ir_program_node_t *ir_program_node = malloc(sizeof(ir_program_node_t));
    if (ast_program_node == NULL) {
        fprintf(stderr, "main: Failed to allocate memory.\n");
        return 1;
    }

    if (generate(ast_program_node, ir_program_node) != 0) {
        fprintf(stderr, "Generating IR failed.\n");
        return 1;
    }

    printf("Generated IR.\n");
    if (until_codegen) {
        printf("Stopping here because --codegen was provided.\n");
        return 0;
    }

    // emitting code
    if (emit("assembly.s", ir_program_node) != 0) {
        fprintf(stderr, "Code emission failed.\n");
        return 1;
    }

    printf("Code emitted.\n");

    // freeing resources
    for (int i = 0; i < tokens_list.count; i++) {
        token_t *t = (token_t *)list_get(&tokens_list, i);
        free(t->value);
        free(t);
    }

    // linking assembly
    if (output_file_path == NULL) {
        source_file_path = get_basename_without_ext(source_file_path);
        snprintf(command, sizeof(command), "gcc assembly.s -o %s", source_file_path);
        free(source_file_path);
    } else {
        snprintf(command, sizeof(command), "gcc assembly.s -o %s", output_file_path);
    }

    status_code = system(command);
    if (status_code != 0) {
        fprintf(stderr, "Something went wrong on the linking step: %d.\n", status_code);
        return 1;
    }

    if (!do_emit) {
        status_code = system("rm assembly.s");
        if (status_code != 0) {
            fprintf(stderr, "Something went wrong on %s deletion: %d.\n", "assembly.s", status_code);
            return 1;
        }
    }

    printf("Linked.\n");
    return 0;
}
