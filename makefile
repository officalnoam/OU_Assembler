assembler: assembler/assembler.exe

assembler/assembler.exe: assembler/io_utils.o assembler/linked_list.o assembler/macro.o assembler/string_utils.o
	gcc -ansi -Wall -pedantic -o assembler.exe assembler/io_utils.o assembler/linked_list.o assembler/macro.o assembler/string_utils.o

assembler/io_utils.o: assembler/io_utils.c assembler/io_utils.h assembler/boolean.h
	gcc -ansi -Wall -pedantic -c assembler/io_utils.c -o assembler/io_utils.o

assembler/linked_list.o: assembler/linked_list.c assembler/linked_list.h assembler/boolean.h
	gcc -ansi -Wall -pedantic -c assembler/linked_list.c -o assembler/linked_list.o

assembler/macro.o: assembler/marco.c assembler/macro.h assembler/linked_list.h
	gcc -ansi -Wall -pedantic -c assembler/macro.c -o assembler/macro.o

assembler/operations.o: assembler/operations.c assembler/operations.h assembler/boolean.h
	gcc -ansi -Wall -pedantic -c assembler/operations.c -o assembler/operations.o

assembler/string_utils.o: assembler/string_utils.c assembler/string_utils.h assembler/boolean.h
	gcc -ansi -Wall -pedantic -c assembler/string_utils.c -o assembler/string_utils.o

assembler/arg_utils.o: assembler/arg_utils.c assembler/arg_utils.h assembler/boolean.h assembler/string_utils.h
	gcc -ansi -Wall -pedantic -c assembler/arg_utils.c -o assembler/arg_utils.o

cleanup:
	rm assembler/*.o
