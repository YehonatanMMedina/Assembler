
#ifndef ASSEMBLERPROJECT_SECOND_PASS_H
#define ASSEMBLERPROJECT_SECOND_PASS_H

#include "globals.h"
#include "table_management.h"


int second_pass(FILE* src_file, CPU * assembler_cpu, symbol_table ** symbols, symbol_table ** extern_symbols, unsigned int* ERROR_FLAG);


unsigned int get_instruction_size_and_addressing_methods(CPU * assembler_cpu, unsigned int IC, unsigned int * destination_addressing_method, unsigned int * source_addressing_method, unsigned int * expected_operands);


void set_symbol_code(CPU * assembler_cpu, unsigned int address, symbol_table * symbol, symbol_table ** extern_symbols, symbol_table ** last_extern_symbol);

void command_is_entry(symbol_table ** symbols, char * current_line, unsigned int * offset, unsigned int line_counter, unsigned int * ERROR_FLAG);


void command_is_instruction(CPU * assembler_cpu, unsigned int * IC, char * current_line, symbol_table ** symbols, symbol_table ** extern_symbols, symbol_table ** last_extern_symbol, unsigned int * offset, unsigned int line_counter, unsigned int* ERROR_FLAG);

#endif /* ASSEMBLERPROJECT_SECOND_PASS_H */
