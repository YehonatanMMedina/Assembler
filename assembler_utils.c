#include <ctype.h>
#include "assembler_utils.h"
#include "globals.h"
#include <stdlib.h>
#include <string.h>


int find_instruction(const char * command) {
    if (strlen(command) == 3) {
        if (strncmp(command, "mov", 3) == 0)
            return mov;
        else if (strncmp(command, "cmp", 3) == 0)
            return cmp;
        else if (strncmp(command, "add", 3) == 0)
            return add;
        else if (strncmp(command, "sub", 3) == 0)
            return sub;
        else if (strncmp(command, "lea", 3) == 0)
            return lea;
        else if (strncmp(command, "not", 3) == 0)
            return not;
        else if (strncmp(command, "clr", 3) == 0)
            return clr;
        else if (strncmp(command, "inc", 3) == 0)
            return inc;
        else if (strncmp(command, "dec", 3) == 0)
            return dec;
        else if (strncmp(command, "jmp", 3) == 0)
            return jmp;
        else if (strncmp(command, "bne", 3) == 0)
            return bne;
        else if (strncmp(command, "red", 3) == 0)
            return red;
        else if (strncmp(command, "prn", 3) == 0)
            return prn;
        else if (strncmp(command, "jsr", 3) == 0)
            return jsr;
        else if (strncmp(command, "rts", 3) == 0)
            return rts;
    }
    if (strlen(command) == 4 && strncmp(command, "stop", 4) == 0)
        return stop;

    else
        return -1;
}

int is_valid_symbol_macro_name(char* name) {
    int i;

    if (strlen(name) > MAX_SYMBOL_LENGTH)
        return 0;

    if (name[0] != '\0' && !isalpha((unsigned char)name[0]))
        return 0;

    for (i = 1; name[i] != '\0'; i++) {
        if (!isalnum((unsigned char)name[i])) {
            return 0;
        }
    }

    if (find_instruction(name) != -1)
        return 0;

    if ((strncmp(name, ".data", 7) == 0) ||
        (strncmp(name, ".string", 7) == 0) ||
        (strncmp(name, ".entry", 7) == 0) ||
        (strncmp(name, ".extern", 7) == 0))
        return 0;

    return 1;
}

memory_cell * encode_instructions(unsigned int ARE, unsigned int destination_operand, unsigned int opcode, unsigned int source_operand) {
    memory_cell * cell = malloc(sizeof(memory_cell));
    if (cell == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    cell->bits = 0;
    cell->bits += ARE;
    cell->bits += destination_operand << 2;
    cell->bits += opcode << 5;
    cell->bits += source_operand << 9;

    return cell;
}

memory_cell * encode_data_parameter(int parameter) {
    memory_cell * cell = malloc(sizeof(memory_cell));
    if (cell == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    cell->bits = parameter;
    return cell;
}

char* base64_encode_12bit(unsigned int num) {
    const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    char *result = malloc(3);
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    result[0] = base64_chars[(num >> 6) & 0x3F];

    result[1] = base64_chars[num & 0x3F];

    result[2] = '\0';

    return result;
}
