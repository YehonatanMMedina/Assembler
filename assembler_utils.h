
#ifndef ASSEMBLERPROJECT_ASSEMBLER_UTILS_H
#define ASSEMBLERPROJECT_ASSEMBLER_UTILS_H

#include "globals.h"


int find_instruction(const char * command);


int is_valid_symbol_macro_name(char* name);


memory_cell * encode_instructions(unsigned int ARE, unsigned int destination_operand, unsigned int opcode, unsigned int source_operand);


memory_cell * encode_data_parameter(int parameter);

char* base64_encode_12bit(unsigned int num);

#endif /* ASSEMBLERPROJECT_ASSEMBLER_UTILS_H */
