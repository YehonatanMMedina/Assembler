#include <stdlib.h>
#include <string.h>
#include "table_management.h"
#include "globals.h"

#include <stdio.h>

void add_macro(macro_table** head, char* name, char* text) {
    macro_table* node = (macro_table*) malloc(sizeof(macro_table));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    node->name = name;
    node->text = text;
    node->next = *head;
    *head = node;
}

char* find_macro(macro_table* head, const char* name) {
    while (head != NULL) {
        if (strcmp(head->name, name) == 0)
            return head->text;
        head = head->next;
    }
    return NULL;
}

void free_macros(macro_table* head) {
    macro_table* tmp;

    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp->name);
        free(tmp->text);
        free(tmp);
    }
}

void add_symbol(symbol_table** head, const char* name, symbol_type type, unsigned int value) {
    symbol_table* node = (symbol_table*) malloc(sizeof(symbol_table));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    strncpy(node->name, name, MAX_SYMBOL_LENGTH - 1);
    node->name[MAX_SYMBOL_LENGTH - 1] = '\0';
    node->type = type;
    node->value = value;
    node->next = *head;
    *head = node;
}

symbol_table * find_symbol_by_name(symbol_table* head, const char* name) {
    while (head != NULL && name != NULL) {
        if (strcmp(head->name, name) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}

symbol_table * find_non_extern_symbol_by_name(symbol_table* head, const char* name) {
    while (head != NULL && name != NULL) {
        if (head->type != extern_symbol && strcmp(head->name, name) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}

symbol_table* copy_symbol(symbol_table* symbol) {

    symbol_table* new_symbol = (symbol_table*) malloc(sizeof(symbol_table));
    if (new_symbol == NULL) {

        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }


    strcpy(new_symbol->name, symbol->name);
    new_symbol->type = symbol->type;
    new_symbol->value = symbol->value;
    new_symbol->next = NULL;

    return new_symbol;
}

symbol_table* find_symbols_by_type(symbol_table* head, symbol_type type, int* list_size) {
    symbol_table* new_head = NULL;
    symbol_table* current = head;
    symbol_table* tmp = NULL;

    *list_size = 0;


    while (current != NULL) {
        if (current->type == type) {

            symbol_table* new_symbol = copy_symbol(current);
            tmp = new_head;
            new_head = new_symbol;
            new_head->next = tmp;
            (*list_size)++;
        }
        current = current->next;
    }
    return new_head;
}

void free_symbols(symbol_table* head) {
    symbol_table* tmp;


    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

void add_ic_to_data_symbols(symbol_table* head, unsigned int IC) {

    while (head != NULL) {
        if (head->type == data_symbol)
            head->value += IC;
        head = head->next;
    }
}