
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "input_utils.h"
#include "error_handler.h"
#include "globals.h"

char * skip_spaces(char * str, unsigned int * offset) {
    while (*str != '\0' && *str != '\n' && isspace((char)*str)) {
        str++;
        (*offset)++;
    }
    return str;
}

char * str_concat_ret(const char * src1, const char * src2) {
    char *str;

    str = malloc((strlen(src1) + strlen(src2) + 1) * sizeof(char)); /* Space for length plus null */
    if (str == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    strcpy(str, src1);
    strcat(str, src2);

    return str;
}

char * read_word(char * str, unsigned int * offset) {
    char* end;
    char* word;
    int len;

    str = skip_spaces(str+(*offset), offset);

    end = str;
    while (*end != '\0' && *end != ' ' && *end != ',' && *end != '\t' && *end != '\n') {
        end++;
    }

    len = (int)(end - str);
    word = (char*)malloc((len + 1) * sizeof(char));
    if (word == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    strncpy(word, str, len);
    word[len] = '\0';

    *offset += len;
    return word;
}

int read_word_or_up_to_char(char * str, char ** word, char c, unsigned int * offset) {
    char* end;
    int len;
    unsigned int offset_temp = *offset;

    str = skip_spaces(str, offset);

    end = str;
    while (*end != c && *end != '\0' && *end != ' ' && *end != ',' && *end != '\t' && *end != '\n') {
        end++;
    }

    if (*end != c) {
        *word = NULL;
        *offset = offset_temp;
        return 0;
    }

    (*offset)++;

    /* Allocate memory for the new string. */
    len = (int)(end - str);
    *word = (char*)malloc((len + 1) * sizeof(char));
    if (word == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    strncpy(*word, str, len);
    (*word)[len] = '\0';

    *offset += len;
    return 1;
}

int read_comma(char * str, unsigned int * offset) {
    str = skip_spaces(str+(*offset), offset);

    if (str[0] != ',')
        return 0;

    (*offset)++;
    return 1;
}

int is_empty(char * str) {
    unsigned int offset = 0;

    str = skip_spaces(str+offset, &offset);

    if ((strlen(str) == 0) || (str[0] == '\n'))
        return 1;
    return 0;
}

int is_empty_or_comment(char * str) {
    unsigned int offset = 0;

    str = skip_spaces(str+offset, &offset);

    if (is_empty(str) || (str[0] == ';'))
        return 1;
    return 0;
}

int is_ascii(int c) {
    return (c >= 0 && c <= 127);
}

int read_next_parameter(char * str, char ** parameter, unsigned int * offset) {
    if (!is_empty(str + *offset))
    {
        if (!read_comma(str, offset))
            return 1;
        else {
            *parameter = read_word(str, offset);
            if (strlen(*parameter) == 0)
                return 2;
            else
                return 3;
        }
    }
    else
        return 0;
}

char * read_string_parameter(char * str, unsigned int * offset, int * ERROR_FLAG) {
    char* word;
    char* end;
    char* ptr;
    unsigned int len;

    str = skip_spaces(str+(*offset), offset);

    if (str[0] != '\"')
        *ERROR_FLAG = 1;

    str++;
    (*offset)++;

    end = str + strlen(str) - 1;
    while (*end != '\"' && end != str)
        end--;

    if (end == str)
        *ERROR_FLAG = 1;

    for (ptr = str; ptr != end; ptr++)
        if (!is_ascii(*ptr))
            *ERROR_FLAG = 1;

    if (*ERROR_FLAG)
        return NULL;

    len = (int)(end - str);
    word = (char*)malloc((len + 1) * sizeof(char));
    if (word == NULL) {
        /* Handle malloc failure */
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    strncpy(word, str, len);
    word[len] = '\0';

    *offset += len;
    return word;
}

int read_operands(char * current_line, char ** first_operand, char ** second_operand, unsigned int * offset, unsigned int * operand_count, unsigned int expected_operands, unsigned int line_counter, unsigned int * ERROR_FLAG) {
    unsigned int COMMA_ERROR_FLAG = 0;

    *operand_count = 0;
    *first_operand = read_word(current_line, offset);

    if (strlen(*first_operand) > 0) {
        (*operand_count)++;
        if ((*operand_count) < expected_operands) {
            if (read_comma(current_line, offset)) {
                *second_operand = read_word(current_line, offset);
                if (strlen(*second_operand) > 0)
                    (*operand_count)++;
            }
            else {
                *second_operand = read_word(current_line, offset);
                if (strlen(*second_operand) > 0)
                    (*operand_count)++;

                COMMA_ERROR_FLAG = 1;
            }
        }
    }

    if ((*operand_count) < expected_operands)
        output_error(one_or_more_operands_missing, line_counter, ERROR_FLAG);
    else if (COMMA_ERROR_FLAG) {
        output_error(missing_comma, line_counter, ERROR_FLAG);
    }
    else if (!is_empty(current_line + (*offset)))
        output_error(instruction_excess_operands, line_counter, ERROR_FLAG);

    return *operand_count == expected_operands;
}

int is_valid_number(const char * parameter) {
    int i;
    int value;

    /* If string is empty its not a number */
    if (strlen(parameter) == 0)
        return 0;

    /* If string is doesnt start with '-' or a digit its not a number */
    if (parameter[0] != '-' && !isdigit((unsigned char)parameter[0]))
        return 0;

    /* If string contains only '-' its not a number */
    if (parameter[0] == '-' && strlen(parameter) == 1)
        return 0;

    /* Check if all characters except the first are digits */
    for (i = 1; parameter[i] != '\0'; i++) {
        if (!isdigit((unsigned char)parameter[i])) {
            return 0;
        }
    }

    /* Check if number is within max and min values */
    value = atoi(parameter);
    if (value > MAX_INT_SIZE || value < MIN_INT_SIZE)
        return 0;

    /* Returns 1 if string is a number, otherwise returns 0 */
    return 1;
}