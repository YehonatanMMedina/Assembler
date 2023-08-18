
#ifndef ASSEMBLERPROJECT_GLOBALS_H
#define ASSEMBLERPROJECT_GLOBALS_H

#include <stdio.h>
#include <stdint.h>
#define MAX_INT_SIZE 2047
#define MIN_INT_SIZE (-2048)

#define MAX_INT_SIZE_IMMEDIATE_ADDRESSING 511
#define MIN_INT_SIZE_IMMEDIATE_ADDRESSING (-512)

#define MEMORY_SIZE 1024
#define NUM_REGISTERS 8
#define MAX_LINE_LENGTH 80
#define MAX_SYMBOL_LENGTH 31
#define INSTRUCTION_COUNTER_INITIAL_ADDRESS 100


typedef struct memory_cell{
    unsigned int bits:12;
} memory_cell;

typedef struct cpu {
    memory_cell instruction_image[1024];
    memory_cell data_image[1024];
} CPU;

typedef enum opname {
    mov, /* 0 */
    cmp, /* 1 */
    add, /* 2 */
    sub, /* 3 */
    not, /* 4 */
    clr, /* 5 */
    lea, /* 6 */
    inc, /* 7 */
    dec, /* 8 */
    jmp, /* 9 */
    bne, /* 10 */
    red, /* 11 */
    prn, /* 12 */
    jsr, /* 13 */
    rts, /* 14 */
    stop /* 15 */
} opname;

typedef enum encoding_type {
    absolute,
    external,
    relocatable
} encoding_type;

typedef enum addressing_mode {
    immediate_addressing = 1,
    direct_addressing = 3,
    register_addressing = 5
} addressing_mode;

typedef enum symbol_type {
    entry_symbol,
    extern_symbol,
    code_symbol,
    data_symbol
} symbol_type;

typedef enum destination_source {
    destination,
    source
} destination_source;

typedef enum ERROR_LIST {
    max_input_line_length_exceeded,
    cpu_out_of_memory,
    memory_allocation_fail,
    file_open_fail,
    illegal_macro_name,
    macro_exists,
    macro_defenition_excess_parameters,
    symbol_name_taken,
    symbol_undefined,
    symbol_with_macro_name,
    illegal_symbol_name,
    missing_command,
    unknown_command,
    one_or_more_operands_missing,
    missing_comma,
    one_or_more_illegal_operands,
    entry_symbol_undefined,
    symbol_already_defined_as_extern,
    instruction_excess_operands,
    illegal_string_definition,
    illegal_register_name,
    illegal_addressing_method_for_instruction
} ERROR_LIST;

typedef enum WARNING_LIST {
    symbol_definition_before_entry,
    symbol_definition_before_extern
} WARNING_LIST;

#endif /* ASSEMBLERPROJECT_GLOBALS_H */
