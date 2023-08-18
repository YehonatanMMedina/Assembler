
#ifndef ASSEMBLERPROJECT_PRE_ASSEMBLER_H
#define ASSEMBLERPROJECT_PRE_ASSEMBLER_H

#include "table_management.h"


int pre_assembler(char* filename, macro_table ** macros, unsigned int * ERROR_FLAG);

int starts_with_mcro(char* str);


int starts_with_endmcro(char* str);

#endif /* ASSEMBLERPROJECT_PRE_ASSEMBLER_H */
