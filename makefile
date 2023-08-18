CC = gcc
EXEC = assembler
OBJS = assembler.o assembler_utils.o error_handler.o file_utils.o first_pass.o input_utils.o pre_assembler.o second_pass.o table_management.o
COMP_FLAG = -ansi -Wall -pedantic
DEBUG_FLAG = -g

$(EXEC): $(OBJS)
	$(CC) $(COMP_FLAG) $(DEBUG_FLAG) $(OBJS) -o $@ -lm

assembler.o: assembler.c pre_assembler.h first_pass.h second_pass.h file_utils.h
	$(CC) -c $(COMP_FLAG) $*.c -o $@

assembler_utils.o: assembler_utils.c assembler_utils.h globals.h
	$(CC) -c $(COMP_FLAG) $*.c -o $@
	
error_handler.o: error_handler.c error_handler.h globals.h
	$(CC) -c $(COMP_FLAG) $*.c -o $@
	
file_utils.o: file_utils.c file_utils.h globals.h input_utils.h table_management.h assembler_utils.h
	$(CC) -c $(COMP_FLAG) $*.c -o $@
	
first_pass.o: first_pass.c first_pass.h globals.h input_utils.h table_management.h assembler_utils.h error_handler.h file_utils.h
	$(CC) -c $(COMP_FLAG) $*.c -o $@
	
input_utils.o: input_utils.c input_utils.h globals.h error_handler.h
	$(CC) -c $(COMP_FLAG) $*.c -o $@
	
pre_assembler.o: pre_assembler.c pre_assembler.h globals.h table_management.h input_utils.h file_utils.h error_handler.h assembler_utils.h
	$(CC) -c $(COMP_FLAG) $*.c -o $@
	
second_pass.o: second_pass.c second_pass.h globals.h table_management.h input_utils.h error_handler.h assembler_utils.h
	$(CC) -c $(COMP_FLAG) $*.c -o $@
	
table_management.o: table_management.c table_management.h globals.h
	$(CC) -c $(COMP_FLAG) $*.c -o $@

clean:
	-rm $(OBJS)

