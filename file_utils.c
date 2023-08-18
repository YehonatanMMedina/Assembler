
#include "input_utils.h"
#include "table_management.h"
#include "assembler_utils.h"
#include <stdlib.h>
#include <string.h>
#include "file_utils.h"
#include "globals.h"


int open_file(FILE** file, char* filename, char* filetype, char* io_mode)
{
    filename = str_concat_ret(filename, filetype);
    if (filename == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 0;
    }
    *file = fopen(filename, io_mode);
    if (*file == NULL) {
        fprintf(stderr, "Unable to open file %s\n", filename);
        free(filename);
        return 0;
    }
    free(filename);
    return 1;
}

int create_object_file(char* filename, CPU * assembler_cpu, const unsigned int * IC, const unsigned int * DC) {
    int i;
    char * code;
    FILE * ob_dst_file;

    if (!open_file(&ob_dst_file, filename, ".ob", "w"))
        return 0;

    fprintf(ob_dst_file, "%d %d\n", *IC - INSTRUCTION_COUNTER_INITIAL_ADDRESS, *DC);

    for (i = INSTRUCTION_COUNTER_INITIAL_ADDRESS; i < (*IC); ++i) {
        code = base64_encode_12bit(assembler_cpu->instruction_image[i].bits);
        fputs(code, ob_dst_file);
        fputs("\n", ob_dst_file);
        free(code);
    }

    for (i = 0; i < (*DC); ++i) {
        code = base64_encode_12bit(assembler_cpu->data_image[i].bits);
        fputs(code, ob_dst_file);
        fputs("\n", ob_dst_file);
        free(code);
    }
    fclose(ob_dst_file);
    return 1;
}

int create_file_for_entry_type(char* filename, symbol_table* symbols, symbol_type type, char* filetype) {
    FILE * ent_dst_file;
    symbol_table* entry_symbols;
    symbol_table* current;
    int size = 0;
    int file_created = 0;

    entry_symbols = find_symbols_by_type(symbols, type, &size);

    if (size > 0) {
        if (!open_file(&ent_dst_file, filename, filetype, "w"))
            return file_created;

        current = entry_symbols;
        while (current != NULL) {
            fprintf(ent_dst_file, "%s\t%d\n", current->name, current->value);
            current = current->next;
        }
        free_symbols(entry_symbols);
        file_created = 1;
        fclose(ent_dst_file);
    }
    return file_created;
}

int create_file_for_extern_type(char* filename, char* filetype, symbol_table * extern_symbols) {
    FILE * ent_dst_file;
    symbol_table *current;
    int file_created = 0;

    if (extern_symbols != NULL) {
        if (!open_file(&ent_dst_file, filename, filetype, "w"))
            return file_created;

        current = extern_symbols;
        while (current != NULL) {
            fprintf(ent_dst_file, "%s\t%d\n", current->name, current->value);
            current = current->next;
        }
        file_created = 1;
        fclose(ent_dst_file);
    }
    return file_created;
}

int is_fgets_buffer_exceeded(FILE * file, char * str) {
    int c;
    if (strchr(str, '\n') == NULL && !feof(file)) {
        while ((c = getc(file)) != '\n' && c != EOF);
        return 1;
    }
    return 0;
}

int is_fgets_buffer_exceeded_copy(FILE * src_file, FILE * dst_file, char * str) {
    int c;
    if (strchr(str, '\n') == NULL && !feof(src_file)) {
        fputs(str, dst_file);
        while ((c = getc(src_file)) != '\n' && c != EOF) {
            putc((char)c, dst_file);
        }
        putc((char)c, dst_file);
        return 1;
    }
    return 0;
}