assembler: assembler/assembler.exe

assembler/assembler.exe: assembler/io_utils.o assembler/linked_list.o assembler/macro.o assembler/string_utils.o assembler/arg_utils.o assembler/operations.o assembler/binary_utils.o assembler/registers.o assembler/file_utils.o assembler/addressing.o assembler/symbol.o assembler/pre_assembler.o assembler/assembler.o
	gcc -ansi -Wall -pedantic -o assembler.exe assembler/io_utils.o assembler/linked_list.o assembler/macro.o assembler/string_utils.o assembler/arg_utils.o assembler/operations.o assembler/binary_utils.o assembler/registers.o assembler/file_utils.o assembler/addressing.o assembler/symbol.o assembler/pre_assembler.o assembler/assembler.o

assembler/io_utils.o: assembler/io_utils.c assembler/io_utils.h assembler/boolean.h
	gcc -ansi -Wall -pedantic -c assembler/io_utils.c -o assembler/io_utils.o

assembler/linked_list.o: assembler/linked_list.c assembler/linked_list.h assembler/boolean.h
	gcc -ansi -Wall -pedantic -c assembler/linked_list.c -o assembler/linked_list.o

assembler/macro.o: assembler/macro.c assembler/macro.h assembler/linked_list.h
	gcc -ansi -Wall -pedantic -c assembler/macro.c -o assembler/macro.o

assembler/operations.o: assembler/operations.c assembler/operations.h assembler/boolean.h assembler/addressing.h
	gcc -ansi -Wall -pedantic -c assembler/operations.c -o assembler/operations.o

assembler/string_utils.o: assembler/string_utils.c assembler/string_utils.h assembler/boolean.h
	gcc -ansi -Wall -pedantic -c assembler/string_utils.c -o assembler/string_utils.o

assembler/arg_utils.o: assembler/arg_utils.c assembler/arg_utils.h assembler/boolean.h assembler/string_utils.h assembler/registers.h assembler/linked_list.h
	gcc -ansi -Wall -pedantic -c assembler/arg_utils.c -o assembler/arg_utils.o

assembler/binary_utils.o: assembler/binary_utils.c assembler/binary_utils.h assembler/boolean.h
	gcc -ansi -Wall -pedantic -c assembler/binary_utils.c -o assembler/binary_utils.o

assembler/registers.o: assembler/registers.c assembler/registers.h
	gcc -ansi -Wall -pedantic -c assembler/registers.c -o assembler/registers.o

assembler/file_utils.o: assembler/file_utils.c assembler/file_utils.h
	gcc -ansi -Wall -pedantic -c assembler/file_utils.c -o assembler/file_utils.o

assembler/addressing.o: assembler/addressing.c assembler/addressing.h assembler/registers.h
	gcc -ansi -Wall -pedantic -c assembler/addressing.c -o assembler/addressing.o

assembler/symbol.o: assembler/symbol.c assembler/symbol.h assembler/linked_list.h
	gcc -ansi -Wall -pedantic -c assembler/symbol.c -o assembler/symbol.o

assembler/pre_assembler.o: assembler/pre_assembler.c assembler/pre_assembler.h assembler/linked_list.h assembler/boolean.h assembler/macro.h assembler/operations.h assembler/arg_utils.h assembler/io_utils.h assembler/file_utils.h
	gcc -ansi -Wall -pedantic -c assembler/pre_assembler.c -o assembler/pre_assembler.o 

assembler/assembler.o: assembler/assembler.c assembler/pre_assembler.h assembler/boolean.h
	gcc -ansi -Wall -pedantic -c assembler/assembler.c -o assembler/assembler.o

cleanup:
	rm assembler/*.o
