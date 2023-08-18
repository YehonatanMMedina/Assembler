
#include "globals.h"
#include "table_management.h"
#include "assembler_utils.h"
#include "error_handler.h"
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include "first_pass.h"
#include "input_utils.h"


#include "file_utils.h"

int first_pass(FILE* src_file, CPU * assembler_cpu, unsigned int * IC, unsigned int * DC, symbol_table **symbols, macro_table * macros, unsigned int* ERROR_FLAG) {
    char current_line[MAX_LINE_LENGTH + 2];
    char *symbol_name = NULL;
    char *command;
    unsigned int SYMBOL_DEF_FLAG;
    unsigned int offset;
    unsigned int line_counter = 0;


    while (((*IC) + (*DC) <= MEMORY_SIZE - 1) && fgets(current_line, MAX_LINE_LENGTH + 2, src_file) != NULL) {
        line_counter++;
        offset = 0;
        SYMBOL_DEF_FLAG = 0;
        symbol_name = NULL;

        if (is_fgets_buffer_exceeded(src_file, current_line)) {
            output_error(max_input_line_length_exceeded, line_counter, ERROR_FLAG);
            continue;
        }

        if (!is_empty_or_comment(current_line)) {
            if (read_word_or_up_to_char(current_line, &symbol_name, ':', &offset)) {
                if (!is_valid_symbol_macro_name(symbol_name)) {
                    output_error(illegal_symbol_name, line_counter, ERROR_FLAG);
                    free(symbol_name);
                    continue;
                }
                else if (find_macro(macros, symbol_name) != NULL) {
                    output_error(symbol_with_macro_name, line_counter, ERROR_FLAG);
                    free(symbol_name);
                    continue;
                }
                else
                    SYMBOL_DEF_FLAG = 1;
            }

            command = read_word(current_line, &offset);
            if (strlen(command) == 0)
                output_error(missing_command, line_counter, ERROR_FLAG);

            else if (!command_is_entry_or_extern(command, current_line, &offset, symbols, SYMBOL_DEF_FLAG, line_counter, ERROR_FLAG)) {

                if (find_symbol_by_name(*symbols, symbol_name) != NULL) {
                    output_error(symbol_name_taken, line_counter, ERROR_FLAG);
                }
                else if (!command_is_data_or_string(assembler_cpu, command, symbol_name, current_line, &offset, symbols, IC, DC, line_counter, ERROR_FLAG, SYMBOL_DEF_FLAG)) {
                    if (SYMBOL_DEF_FLAG) {
                        add_symbol(symbols, symbol_name, code_symbol, *IC);
                    }

                    process_instruction(assembler_cpu, IC, DC, current_line, command, &offset, line_counter, ERROR_FLAG);
                }
            }
            if (SYMBOL_DEF_FLAG)
                free(symbol_name);
            free(command);
        }
    }

    add_ic_to_data_symbols(*symbols, *IC);

    if (*ERROR_FLAG)
        return 0;
    return 1;
}

int is_parameter_number(const char * str, int *value) {
    char *end;
    long val;
    if (is_valid_number(str)) {
        val = strtol(str, &end, 10);
        *value = (int) val;
        return 1;
    }
    return 0;
}

int is_parameter_register(const char * str, int * operand_as_register, unsigned int * ERROR_FLAG) {
    char *endptr;
    int value;

    if (strlen(str) >= 1 && str[0] == '@')
    {
        value = strtol(str + 2, &endptr, 10);
        if ((strlen(str) != 3) || (str[1] != 'r') || (value < 0) || (value > NUM_REGISTERS - 1))
        {
            *ERROR_FLAG = 1;
            return 1;
        }
    }
    else
        return 0;

    *operand_as_register = value;
    return 1;
}

int get_addressing_method(char * operand, int * operand_as_number, int * operand_as_register) {
    unsigned int REGISTER_ERROR_FLAG = 0;

    if (is_parameter_number(operand, operand_as_number))
        return immediate_addressing;
    else if (is_parameter_register(operand, operand_as_register, &REGISTER_ERROR_FLAG))
    {
        if (REGISTER_ERROR_FLAG)
            return 0;
        return register_addressing;
    }
    else if (is_valid_symbol_macro_name(operand))
        return direct_addressing;
    return 0;
}

memory_cell * process_operand(char * operand, int * addressing_method, int dest_src, unsigned int* ERROR_FLAG) {
    memory_cell * operand_machine_code;
    int operand_as_number;
    int operand_as_register;

    operand_machine_code = malloc(sizeof(memory_cell));
    if (operand_machine_code == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    *addressing_method = get_addressing_method(operand, &operand_as_number, &operand_as_register);

    if (*addressing_method == immediate_addressing)
    {
        if (operand_as_number < MIN_INT_SIZE_IMMEDIATE_ADDRESSING || operand_as_number > MAX_INT_SIZE_IMMEDIATE_ADDRESSING)
            *ERROR_FLAG = 1;
        else {
            operand_machine_code->bits = absolute;
            operand_machine_code->bits += operand_as_number << 2;
        }
    }
    else if (*addressing_method == direct_addressing)
    {
        free(operand_machine_code);
        return NULL;
    }
    else if (*addressing_method == register_addressing)
    {
        operand_machine_code->bits = absolute;
        if (dest_src == destination)
            operand_machine_code->bits += operand_as_register << 2;
        if (dest_src == source)
            operand_machine_code->bits += (unsigned int)operand_as_register << 7;
    }
    else
        *ERROR_FLAG = 1;
    return operand_machine_code;
}

memory_cell * merge_register_codes(memory_cell * first_code, memory_cell * second_code) {
    first_code->bits |= second_code->bits;
    return first_code;
}

void process_instruction(CPU * assembler_cpu, unsigned int * IC, unsigned int * DC, char * current_line, char * command, unsigned int * offset,unsigned int line_counter, unsigned int* ERROR_FLAG) {
    char * first_operand = NULL;
    char * second_operand = NULL;
    int instruction;
    unsigned int operand_count = 0;
    unsigned int L = 0;

    instruction = find_instruction(command);
    if (instruction == -1) {
        output_error(unknown_command, line_counter, ERROR_FLAG);
    }
    else {
        if ((instruction >= mov) && (instruction <= lea)) {
            if (read_operands(current_line, &first_operand, &second_operand, offset, &operand_count, 2, line_counter, ERROR_FLAG)) {
                save_instruction_to_memory(assembler_cpu, IC, DC, first_operand, second_operand, instruction, 2, &L, line_counter, ERROR_FLAG);
            }
        }
        else if ((instruction >= not) && (instruction <= jsr)) {
            if (read_operands(current_line, &first_operand, &second_operand, offset, &operand_count, 1, line_counter, ERROR_FLAG)) {
                save_instruction_to_memory(assembler_cpu, IC, DC, first_operand, second_operand, instruction, 1, &L, line_counter, ERROR_FLAG);
            }
        }
        else if ((instruction >= rts) && (instruction <= stop)) {
            if (read_operands(current_line, &first_operand, &second_operand, offset, &operand_count, 0, line_counter, ERROR_FLAG)) {
                save_instruction_to_memory(assembler_cpu, IC, DC, first_operand, second_operand, instruction, 0, &L, line_counter, ERROR_FLAG);
            }
        }

        if (first_operand != NULL)
            free(first_operand);
        if (second_operand != NULL)
            free(second_operand);
    }
}

int validate_addressing_method_per_instruction(int instruction, int  source_addressing_method, int  destination_addressing_method) {
    switch (instruction) {
        case mov:
        case add:
        case sub:
        case not:
        case clr:
        case lea:
        case inc:
        case dec:
        case jmp:
        case bne:
        case red:
        case jsr:
            if (destination_addressing_method == immediate_addressing) return 0;
            break;
    }
    if (instruction == lea && source_addressing_method != direct_addressing)
        return 0;

    return 1;
}

void save_instruction_to_memory(CPU * assembler_cpu, unsigned int * IC, const unsigned int * DC, char * first_operand, char * second_operand, int instruction, int operands_count, unsigned int * L, unsigned int line_counter, unsigned int* ERROR_FLAG) {
    int source_addressing_method = 0;
    int destination_addressing_method = 0;
    unsigned int OPERAND_ERROR_FLAG = 0;
    memory_cell * first_code = NULL;
    memory_cell * second_code = NULL;
    memory_cell * cell;

    if (operands_count == 2) {
        first_code = process_operand(first_operand, &source_addressing_method, source, &OPERAND_ERROR_FLAG);
        second_code = process_operand(second_operand, &destination_addressing_method, destination, &OPERAND_ERROR_FLAG);
    }
    else if (operands_count == 1)
        first_code = process_operand(first_operand, &destination_addressing_method, destination, &OPERAND_ERROR_FLAG);

    if (OPERAND_ERROR_FLAG)
        output_error(one_or_more_illegal_operands, line_counter, ERROR_FLAG);
    else if (!validate_addressing_method_per_instruction(instruction, source_addressing_method, destination_addressing_method))
        output_error(illegal_addressing_method_for_instruction, line_counter, ERROR_FLAG);

    (*L) = operands_count + 1;

    if ((operands_count == 2) && (source_addressing_method == register_addressing) && (destination_addressing_method == register_addressing)) {
        first_code = merge_register_codes(first_code, second_code);
        (*L)--;
    }

    if ((*IC) + (*DC) +(*L) > MEMORY_SIZE - 1)
        output_error(cpu_out_of_memory, line_counter, ERROR_FLAG);
    else {
        cell = encode_instructions(absolute,
                                               destination_addressing_method,
                                               instruction,
                                               source_addressing_method);
        assembler_cpu->instruction_image[(*IC)].bits = cell->bits;
        if (((*L) >= 2) && (first_code != NULL))
            assembler_cpu->instruction_image[(*IC) + 1].bits = first_code->bits;
        if (((*L) == 3) && (second_code != NULL))
            assembler_cpu->instruction_image[(*IC) + 2].bits = second_code->bits;

        (*IC) += (*L);
        free(cell);
    }
    if (first_code != NULL)
        free(first_code);
    if (second_code != NULL)
        free(second_code);
}

void proccess_data(CPU * assembler_cpu, const unsigned int * IC, unsigned int * DC, char * current_line, unsigned int * offset, unsigned int line_counter, unsigned int * ERROR_FLAG) {
    memory_cell * cell;
    char * parameter;
    unsigned int PARAMETER_ERROR_FLAG = 0;
    int parameter_count = 0;
    int parameter_read_status;

    parameter = read_word(current_line, offset);
    while (parameter != NULL && strlen(parameter) > 0 && !PARAMETER_ERROR_FLAG && ((*IC) + (*DC) <= MEMORY_SIZE - 1)) {
        parameter_count++;

        if (is_valid_number(parameter)) {
            cell = encode_data_parameter(atoi(parameter));
            assembler_cpu->data_image[(*DC)].bits = cell->bits;
            free(cell);
            free(parameter);
            parameter = NULL;


            parameter_read_status = read_next_parameter(current_line, &parameter, offset);
            if (parameter_read_status == 1) {
                output_error(missing_comma, line_counter, ERROR_FLAG);
                PARAMETER_ERROR_FLAG = 1;
            }
            if (parameter_read_status == 2) {
                output_error(one_or_more_operands_missing, line_counter, ERROR_FLAG);
                PARAMETER_ERROR_FLAG = 1;
            }

            (*DC)++;
            if ((*IC) + (*DC) > MEMORY_SIZE - 1)
                output_error(cpu_out_of_memory, line_counter, ERROR_FLAG);
        }
        else {
            output_error(one_or_more_illegal_operands, line_counter, ERROR_FLAG);
            PARAMETER_ERROR_FLAG = 1;
        }
    }
    if (parameter_count == 0)
        output_error(one_or_more_operands_missing, line_counter, ERROR_FLAG);
}

int proccess_string(CPU * assembler_cpu, const unsigned int * IC, unsigned int * DC, char * current_line, unsigned int * offset, unsigned int line_counter, unsigned int * ERROR_FLAG) {
    memory_cell * cell;
    int STR_ERROR_FLAG = 0;
    char * str;
    char * str_ptr;

    str = read_string_parameter(current_line, offset, &STR_ERROR_FLAG);
    str_ptr = str;

    if (STR_ERROR_FLAG) {
        output_error(illegal_string_definition, line_counter, ERROR_FLAG);
        return 0;
    }

    while (strlen(str_ptr) > 0 && (*IC) + (*DC) <= MEMORY_SIZE - 1) {
        cell = encode_data_parameter(str_ptr[0]);
        assembler_cpu->data_image[(*DC)].bits = cell->bits;
        free(cell);
        str_ptr++;
        (*DC)++;
    }

    if ((*IC) + (*DC) <= MEMORY_SIZE - 1) {
        cell = encode_data_parameter('\0');
        assembler_cpu->data_image[(*DC)].bits = cell->bits;
        (*DC)++;
        free(cell);
    }
    free(str);

    if ((*IC) + (*DC) > MEMORY_SIZE - 1) {
        output_error(cpu_out_of_memory, line_counter, ERROR_FLAG);
        return 0;
    }
    return 1;
}

int command_is_entry_or_extern(char * command, char* current_line, unsigned int * offset, symbol_table **symbols, unsigned int SYMBOL_DEF_FLAG, unsigned int line_counter, unsigned int * ERROR_FLAG) {
    symbol_table * symbol;
    char * parameter;
    int parameter_read_status;
    int PARAMETER_ERROR_FLAG = 0;
    int SYMBOL_ERROR_FLAG = 0;
    int parameter_count = 0;

    if (strncmp(command, ".entry", 7) == 0) {
        return 1;
    }
    else if (strncmp(command, ".extern", 8) == 0) {

        if (SYMBOL_DEF_FLAG)

            output_warning(symbol_definition_before_extern, line_counter);

        parameter = read_word(current_line, offset);
        while (parameter != NULL && strlen(parameter) > 0 && !PARAMETER_ERROR_FLAG && !SYMBOL_ERROR_FLAG) {
            parameter_count++;

            if (is_valid_symbol_macro_name(parameter)) {
                symbol = find_non_extern_symbol_by_name(*symbols, parameter);
                if (symbol == NULL)
                    add_symbol(symbols, parameter, extern_symbol, 0);
                else {
                    output_error(symbol_name_taken, line_counter, ERROR_FLAG);
                    SYMBOL_ERROR_FLAG = 1;
                }

                free(parameter);
                parameter = NULL;


                parameter_read_status = read_next_parameter(current_line, &parameter, offset);
                if (parameter_read_status == 1) {
                    output_error(missing_comma, line_counter, ERROR_FLAG);
                    PARAMETER_ERROR_FLAG = 1;
                }
                if (parameter_read_status == 2) {
                    output_error(one_or_more_operands_missing, line_counter, ERROR_FLAG);
                    PARAMETER_ERROR_FLAG = 1;
                }
            }
            else {
                output_error(one_or_more_illegal_operands, line_counter, ERROR_FLAG);
                PARAMETER_ERROR_FLAG = 1;
            }
        }

        if (parameter != NULL)
            free(parameter);

        if (parameter_count == 0)
            output_error(one_or_more_operands_missing, line_counter, ERROR_FLAG);

        return 1;
    }
    return 0;
}

int command_is_data_or_string(CPU * assembler_cpu, char * command, char * symbol_name, char* current_line, unsigned int * offset, symbol_table **symbols , unsigned int * IC, unsigned int * DC, unsigned int line_counter, unsigned int * ERROR_FLAG, unsigned int SYMBOL_DEF_FLAG) {
    if (strncmp(command, ".data", 6) == 0) {
        if (SYMBOL_DEF_FLAG)
            add_symbol(symbols, symbol_name, data_symbol, *DC);
        proccess_data(assembler_cpu, IC, DC, current_line, offset, line_counter, ERROR_FLAG);
        return 1;
    } else if (strncmp(command, ".string", 8) == 0) {
        if (SYMBOL_DEF_FLAG)
            add_symbol(symbols, symbol_name, data_symbol, *DC);
        proccess_string(assembler_cpu, IC, DC, current_line, offset, line_counter, ERROR_FLAG);
        return 1;
    }
    return 0;
}
