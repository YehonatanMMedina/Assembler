#include "globals.h"

#include "error_handler.h"
void output_warning(unsigned int warning, unsigned int line_number) {
    switch (warning) {
        case symbol_definition_before_entry:
            fprintf(stderr, "Warning, symbol defined before \".entry\" command in line %d.\n",
                    line_number);
            break;
        case symbol_definition_before_extern:
            fprintf(stderr, "Warning, symbol defined before \".extern\" command in line %d.\n",
                    line_number);
            break;
    }
}
void output_error(unsigned int error, unsigned int line_number, unsigned int * ERROR_FLAG) {
    switch (error) {
        case max_input_line_length_exceeded:
            fprintf(stderr, "Error in line %d: Input line length exceeds max allowed length of %d characters\n", line_number, MAX_LINE_LENGTH); /* wrong no need for line number*/
            *ERROR_FLAG = 1;
            break;
        case cpu_out_of_memory:
            fprintf(stderr, "Error in line %d: CPU ran out of memory\n", line_number); /* wrong no need for line number*/
            *ERROR_FLAG = 1;
            break;
        case memory_allocation_fail:
            fprintf(stderr, "Error in line %d: Memory allocation failed\n", line_number); /* wrong no need for line number*/
            *ERROR_FLAG = 1;
            break;
        case file_open_fail:
            fprintf(stderr, "Error in line %d: Unable to open file\n", line_number); /* wrong no need for line number */
            *ERROR_FLAG = 1;
            break;
        case illegal_macro_name:
            fprintf(stderr, "Error in line %d: Illegal macro name\n", line_number);
            *ERROR_FLAG = 1;
            break;
        case macro_exists:
            fprintf(stderr, "Error in line %d: Macro name already exists\n", line_number);
            *ERROR_FLAG = 1;
            break;
        case macro_defenition_excess_parameters:
            fprintf(stderr, "Error in line %d: Excess parameters after macro definition\n", line_number);
            *ERROR_FLAG = 1;
            break;
        case symbol_name_taken:
            fprintf(stderr, "Error in line %d: Symbol name already exists or is defined as external\n", line_number);
            *ERROR_FLAG = 1;
            break;
        case symbol_undefined:
            fprintf(stderr, "Error in line %d: Symbol operand is undefined\n", line_number);
            *ERROR_FLAG = 1;
            break;
        case symbol_with_macro_name:
            fprintf(stderr, "Error in line %d: Symbol name previously used for macro definition\n", line_number);
            *ERROR_FLAG = 1;
            break;
        case illegal_symbol_name:
            fprintf(stderr, "Error in line %d: Illegal symbol name\n", line_number);
            *ERROR_FLAG = 1;
            break;
        case missing_command:
            fprintf(stderr, "Error in line %d: Missing command\n", line_number);
            *ERROR_FLAG = 1;
            break;
        case unknown_command:
            fprintf(stderr, "Error in line %d: Unknown command\n", line_number);
            *ERROR_FLAG = 1;
            break;
        case one_or_more_operands_missing:
            fprintf(stderr, "Error in line %d: One or more operands missing\n", line_number);
            *ERROR_FLAG = 1;
            break;
        case missing_comma:
            fprintf(stderr, "Error in line %d: missing comma between parameters\n", line_number);
            *ERROR_FLAG = 1;
            break;
        case one_or_more_illegal_operands:
            fprintf(stderr, "Error in line %d: One or more illegal operands\n", line_number);
            *ERROR_FLAG = 1;
            break;
        case entry_symbol_undefined:
            fprintf(stderr, "Error in line %d: Entry symbol is undefined\n", line_number);
            *ERROR_FLAG = 1;
            break;
        case symbol_already_defined_as_extern:
            fprintf(stderr, "Error in line %d: Symbol is already defined as data type or as external\n", line_number);
            *ERROR_FLAG = 1;
            break;
        case instruction_excess_operands:
            fprintf(stderr, "Error in line %d: Instruction received excess operands\n", line_number);
            *ERROR_FLAG = 1;
            break;
        case illegal_string_definition:
            fprintf(stderr, "Error in line %d: String is either illegally defined or contains illegal characters\n", line_number);
            *ERROR_FLAG = 1;
            break;
        case illegal_register_name:
            fprintf(stderr, "Error in line %d: Illegal register name\n", line_number);
            *ERROR_FLAG = 1;
            break;
        case illegal_addressing_method_for_instruction:
            fprintf(stderr, "Error in line %d: Illegal addressing method for instruction\n", line_number);
            *ERROR_FLAG = 1;
            break;
    }
}

