#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_basename_without_ext(const char *path) {
    const char *last_slash = strrchr(path, '/');
    const char *last_backslash = strrchr(path, '\\');

    const char *filename_start = path;
    if (last_slash != NULL && last_slash > last_backslash) {
        filename_start = last_slash + 1;
    } else if (last_backslash != NULL) {
        filename_start = last_backslash + 1;
    }

    char *basename = strdup(filename_start);
    if (basename == NULL) { return NULL; }

    char *last_dot = strrchr(basename, '.');
    if (last_dot != NULL && last_dot != basename) { *last_dot = '\0'; }

    return basename;
}

int main(int argc, char **argv) {
    int until_lexer = 0;
    int until_parser = 0;
    int until_codegen = 0;
    char *source_file_path = NULL;

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
        fprintf(stderr, "Ambigous: can stop either after lexer, parser, or code generator.\n");
        return 1;
    }

    char command[512];
    int status_code = 0;

    snprintf(command, sizeof(command), "gcc -E -P %s -o preprocessed.i", source_file_path);
    status_code = system(command);
    if (status_code != 0) {
        fprintf(stderr, "Something went wrong on the preprocessing step: %d.\n", status_code);
        return 1;
    }

    printf("Preprocessed.\n");

    // here is where compiler will live, stubbing with GCC for now
    status_code = system("gcc -S preprocessed.i -o assembly.s");
    if (status_code != 0) {
        fprintf(stderr, "Something went wrong on the compilation step: %d.\n", status_code);
        return 1;
    }
    status_code = system("rm preprocessed.i");
    if (status_code != 0) {
        fprintf(stderr, "Something went wrong on %s deletion: %d.\n", "preprocessed.i", status_code);
        return 1;
    }

    printf("Compiled.\n");

    source_file_path = get_basename_without_ext(source_file_path);
    snprintf(command, sizeof(command), "gcc assembly.s -o %s", source_file_path);
    status_code = system(command);
    if (status_code != 0) {
        fprintf(stderr, "Something went wrong on the linking step: %d.\n", status_code);
        return 1;
    }
    status_code = system("rm assembly.s");
    if (status_code != 0) {
        fprintf(stderr, "Something went wrong on %s deletion: %d.\n", "assembly.s", status_code);
        return 1;
    }

    printf("Linked.\n");
    return 0;
}
