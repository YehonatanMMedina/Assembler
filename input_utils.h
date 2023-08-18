
#ifndef ASSEMBLERPROJECT_INPUT_UTILS_H
#define ASSEMBLERPROJECT_INPUT_UTILS_H

char* skip_spaces(char* str, unsigned int * offset);


char * str_concat_ret(const char *src1, const char *src2);


char * read_word(char * str, unsigned int * offset);


int read_word_or_up_to_char(char * str, char ** word, char c, unsigned int * offset);


int read_comma(char * str, unsigned int * offset);


int is_empty(char * str);

int is_empty_or_comment(char * str);


int is_ascii(int c);


int read_next_parameter(char * str, char ** parameter, unsigned int * offset);


char * read_string_parameter(char * str, unsigned int * offset, int * ERROR_FLAG);


int read_operands(char * current_line, char ** first_operand, char ** second_operand, unsigned int * offset, unsigned int * operand_count, unsigned int expected_operands, unsigned int line_counter, unsigned int * ERROR_FLAG);

int is_valid_number(const char * parameter);

#endif /* ASSEMBLERPROJECT_INPUT_UTILS_H */
