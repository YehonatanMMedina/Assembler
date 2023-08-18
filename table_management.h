
#ifndef ASSEMBLERPROJECT_TABLE_MANAGEMENT_H
#define ASSEMBLERPROJECT_TABLE_MANAGEMENT_H

#include "globals.h"

typedef struct macro_table {
    char * name; /* Macro name. */
    char * text; /* Macro text. */
    struct macro_table* next; /* Pointer to the next entry in the table. */
} macro_table;

typedef struct symbol_table {
    char name[MAX_SYMBOL_LENGTH]; /* symbol name. */
    symbol_type type; /* Type of the symbol. */
    unsigned int value; /* Symbol address. */
    struct symbol_table* next; /* Pointer to the next entry in the table. */
} symbol_table;


void add_macro(macro_table** head, char* name, char* text);


char* find_macro(macro_table* head, const char* name);

void free_macros(macro_table* head);

void add_symbol(symbol_table** head, const char* name, symbol_type type, unsigned int value);

symbol_table * find_symbol_by_name(symbol_table* head, const char* name);

symbol_table * find_non_extern_symbol_by_name(symbol_table* head, const char* name);

symbol_table* copy_symbol(symbol_table* symbol);

symbol_table* find_symbols_by_type(symbol_table* head, symbol_type type, int* list_size);

void free_symbols(symbol_table* head);


void add_ic_to_data_symbols(symbol_table* head, unsigned int IC);

#endif /* ASSEMBLERPROJECT_TABLE_MANAGEMENT_H */
