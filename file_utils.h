
#ifndef ASSEMBLERPROJECT_FILE_UTILS_H
#define ASSEMBLERPROJECT_FILE_UTILS_H

#include "globals.h"
#include "table_management.h"

int open_file(FILE** file, char* filename, char* filetype, char* io_mode);

int create_object_file(char* filename, CPU * assembler_cpu, const unsigned int * IC, const unsigned int * DC);


int create_file_for_entry_type(char* filename, symbol_table* symbols, symbol_type type, char* filetype);

int create_file_for_extern_type(char* filename, char* filetype, symbol_table * extern_symbols);


int is_fgets_buffer_exceeded(FILE * file, char * str);


int is_fgets_buffer_exceeded_copy(FILE * src_file, FILE * dst_file, char * str);

#endif /* ASSEMBLERPROJECT_FILE_UTILS_H */
