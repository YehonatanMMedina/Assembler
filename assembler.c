#include <stdint.h>
#include "pre_assembler.h"
#include "first_pass.h"
#include "second_pass.h"
#include "file_utils.h"


 int assembler(char * filename);

int main(int argc, char *argv[]) {
    int i;
    if (argc < 2) {
        printf("Invalid number of arguments.\n");
        return 1; /* Return an error code */
    }

    for (i = 1; i < argc; ++i) {
        assembler(argv[i]);
    }
    return 0;
}


int assembler(char * filename) {

    unsigned int IC = INSTRUCTION_COUNTER_INITIAL_ADDRESS;
    unsigned int DC = 0;
    unsigned int ERROR_FLAG = 0;


    symbol_table *symbols = NULL;
    symbol_table *extern_symbols = NULL;
    macro_table * macros = NULL;
    FILE * as_src_file;
    CPU assembler_cpu;



    pre_assembler(filename, &macros, &ERROR_FLAG);


    if (ERROR_FLAG)
        return 0;


    if (!open_file(&as_src_file, filename, ".am", "r"))
        return 0;


    if (!ERROR_FLAG) {
        first_pass(as_src_file, &assembler_cpu, &IC, &DC, &symbols, macros, &ERROR_FLAG);

    }


    if (!ERROR_FLAG) {
        second_pass(as_src_file, &assembler_cpu, &symbols, &extern_symbols, &ERROR_FLAG);

    }


    if (!ERROR_FLAG) {
            printf("Object file %s.ob created.\n", filename);
    }


    if (!ERROR_FLAG) {
        create_file_for_entry_type(filename, symbols, entry_symbol, ".ent");
    }


    if (!ERROR_FLAG) {
        create_file_for_extern_type(filename, ".ext", extern_symbols);

    }

    printf("\n");
    free_macros(macros);
    free_symbols(symbols);
    free_symbols(extern_symbols);
    fclose(as_src_file);

    if (ERROR_FLAG)
        return 0;
    return 1;
}
