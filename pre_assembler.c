#include <string.h>
#include "pre_assembler.h"
#include "globals.h"
#include "table_management.h"
#include "input_utils.h"
#include "file_utils.h"
#include <stdio.h>
#include <stdlib.h>

#include "error_handler.h"
#include "assembler_utils.h"

int pre_assembler(char* filename, macro_table ** macros, unsigned int* ERROR_FLAG) {
    FILE *as_src_file, * as_dst_file;
    char current_line[MAX_LINE_LENGTH + 2];
    char *first_word;
    char *macro_name;
    char *macro_buffer;
    char *macro_text;
    unsigned int macro_buffer_line_count = 0;
    unsigned int offset;
    unsigned int line_counter = 0;
    unsigned int MCRO_FLAG = 0;


    if (!(open_file(&as_src_file, filename, ".as", "r") && open_file(&as_dst_file, filename, ".am", "w"))) {
        *ERROR_FLAG = 1;
        return 0;
    }

    while (fgets(current_line, MAX_LINE_LENGTH + 2, as_src_file) != NULL) {

        if (is_fgets_buffer_exceeded_copy(as_src_file, as_dst_file, current_line))
            continue;

        offset = 0;
        line_counter++;


        first_word = read_word(current_line, &offset);
        if (MCRO_FLAG && starts_with_endmcro(current_line)) {
            if (!is_empty(current_line + 7))
                output_error(macro_defenition_excess_parameters, line_counter, ERROR_FLAG);
            else
                add_macro(macros, macro_name, macro_buffer);

            macro_buffer_line_count = 0;
            MCRO_FLAG = 0;
        }
        else if (MCRO_FLAG) {
            macro_buffer_line_count++;

            macro_buffer = (char*) realloc(macro_buffer, macro_buffer_line_count * MAX_LINE_LENGTH * sizeof(char));
            if (macro_buffer == NULL) {
                output_error(memory_allocation_fail, line_counter, ERROR_FLAG);
                fclose(as_src_file);
                fclose(as_dst_file);
                return 0;
            }

            strcat(macro_buffer, current_line);
        }
        else if (starts_with_mcro(current_line)) {

            macro_name = read_word(current_line, &offset);

            if (find_macro(*macros, macro_name) != NULL) {
                output_error(macro_exists, line_counter, ERROR_FLAG);
            }
            else if (!is_valid_symbol_macro_name(macro_name)) {
                output_error(illegal_macro_name, line_counter, ERROR_FLAG);
            }
            else if (!is_empty(current_line + offset)) {
                output_error(macro_defenition_excess_parameters, line_counter, ERROR_FLAG);
            }
            else {
                MCRO_FLAG = 1;
                macro_buffer = (char*) malloc(MAX_LINE_LENGTH * sizeof(char));
                if (macro_buffer == NULL) {
                    output_error(memory_allocation_fail, line_counter, ERROR_FLAG);
                    fclose(as_src_file);
                    fclose(as_dst_file);
                    free(macro_name);
                    return 0;
                }
                macro_buffer[0] = '\0';
            }
            if (*ERROR_FLAG)
                free(macro_name);
        }
        else {
            macro_text = find_macro(*macros, first_word);

            if (macro_text != NULL)
                fputs(macro_text, as_dst_file);
            else
                fputs(current_line, as_dst_file);
        }
        free(first_word);
    }

    fclose(as_src_file);
    fclose(as_dst_file);

    if (*ERROR_FLAG) {
        filename = str_concat_ret(filename, ".am");
        remove(filename);
        free(filename);
        return 0;
    }
    return 1;
}

int starts_with_mcro(char* str) {
    unsigned int offset = 0;

    /* Skip initial spaces and tabs. */
    str = skip_spaces(str, &offset);

    /* Returns 1 if begins with mcro */
    return strncmp(str, "mcro", 4) == 0;
}

int starts_with_endmcro(char* str) {
    unsigned int offset = 0;

    /* Skip initial spaces and tabs. */
    str = skip_spaces(str, &offset);

    /* Returns 1 if begins with endmcro */
    return (strncmp(str, "endmcro", 7) == 0);
}

