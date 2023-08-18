
#ifndef ASSEMBLERPROJECT_FIRST_PASS_H
#define ASSEMBLERPROJECT_FIRST_PASS_H

#include "globals.h"
#include "table_management.h"


int first_pass(FILE* src_file, CPU* assembler_cpu, unsigned int* IC, unsigned int* DC, symbol_table** symbols, macro_table* macros, unsigned int* ERROR_FLAG);


int is_parameter_number(const char* str, int* value);


int is_parameter_register(const char* str, int* operand_as_register, unsigned int* ERROR_FLAG);

int get_addressing_method(char* operand, int* operand_as_number, int* operand_as_register);

memory_cell * process_operand(char* operand, int* addressing_method, int dest_src, unsigned int* ERROR_FLAG);


memory_cell * merge_register_codes(memory_cell * first_code, memory_cell * second_code);

void process_instruction(CPU * assembler_cpu, unsigned int * IC, unsigned int * DC, char * current_line, char * command, unsigned int * offset,unsigned int line_counter, unsigned int* ERROR_FLAG);

int validate_addressing_method_per_instruction(int instruction, int source_addressing_method, int destination_addressing_method);

void save_instruction_to_memory(CPU * assembler_cpu, unsigned int * IC, const unsigned int * DC, char * first_operand, char * second_operand, int instruction, int operands_count, unsigned int * L, unsigned int line_counter, unsigned int* ERROR_FLAG);

void proccess_data(CPU * assembler_cpu, const unsigned int * IC, unsigned int * DC, char * current_line, unsigned int * offset, unsigned int line_counter, unsigned int * ERROR_FLAG);

int proccess_string(CPU * assembler_cpu, const unsigned int * IC, unsigned int * DC, char * current_line, unsigned int * offset, unsigned int line_counter, unsigned int * ERROR_FLAG);

int command_is_entry_or_extern(char * command, char* current_line, unsigned int * offset, symbol_table **symbols, unsigned int SYMBOL_DEF_FLAG, unsigned int line_counter, unsigned int * ERROR_FLAG);

int command_is_data_or_string(CPU * assembler_cpu, char * command, char * symbol_name, char* current_line, unsigned int * offset, symbol_table **symbols, unsigned int * IC, unsigned int * DC, unsigned int line_counter, unsigned int * ERROR_FLAG, unsigned int SYMBOL_DEF_FLAG);

#endif /* ASSEMBLERPROJECT_FIRST_PASS_H */
