#include "emitter.h"
#include <stdio.h>

int emit_operand(FILE *output, ir_operand_node_t *operand) {
    switch (operand->type) {
    case IR_OPERAND_IMMEDIATE:
        fprintf(output, "$%d", operand->data.immediate.value);
        break;
    case IR_OPERAND_REGISTER:
        fprintf(output, "%%eax");
        break;
    }
    return 0;
}

int emit_instruction(FILE *output, ir_instruction_node_t *instruction) {
    switch (instruction->type) {
    case IR_INSTRUCTION_RET:
        fprintf(output, "    ret\n");
        break;
    case IR_INSTRUCTION_MOV:
        fprintf(output, "    movl ");
        if (emit_operand(output, instruction->data.mov.left) != 0) {
            fprintf(stderr, "Failed to emit left operand of mov instruction.\n");
            return -1;
        }
        fprintf(output, ", ");
        if (emit_operand(output, instruction->data.mov.right) != 0) {
            fprintf(stderr, "Failed to emit right operand of mov instruction.\n");
            return -1;
        }
        fprintf(output, "\n");
        break;
    }

    return 0;
}

int emit_function(FILE *output, ir_function_node_t *function) {
    fprintf(output, "%s:\n", function->name);

    for (int i = 0; i < function->instructions.count; i++) {
        if (emit_instruction(output, (ir_instruction_node_t *)list_get(&function->instructions, i)) != 0) {
            fprintf(stderr, "Failed to emit instruction into function %s.\n", function->name);
            return -1;
        }
    }

    return 0;
}

int emit(char *filename, ir_program_node_t *program) {
    FILE *output;
    output = fopen(filename, "w");
    if (output == NULL) {
        fprintf(stderr, "Failed to open file %s.\n", filename);
        return -1;
    }

    fprintf(output, "    .globl main\n");

    for (int i = 0; i < program->functions.count; i++) {
        if (emit_function(output, (ir_function_node_t *)list_get(&program->functions, i)) != 0) {
            fprintf(stderr, "Failed to emit function into %s.\n", filename);
            return -1;
        }
    }

    fprintf(output, "    .section .note.GNU-stack,\"\",@progbits\n");
    fclose(output);
    return 0;
}
