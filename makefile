assembler: assembler/assembler.exe

assembler/assembler.exe: assembler/io_utils.o assembler/linked_list.o
	gcc -ansi -Wall -pedantic -o assembler.exe assembler/io_utils.o assembler/linked_list.o

assembler/io_utils.o: assembler/io_utils.c assembler/io_utils.h assembler/boolean.h
	gcc -ansi -Wall -pedantic -c assembler/io_utils.c -o assembler/io_utils.o

assembler/linked_list.o: assembler/linked_list.c assembler/linked_list.h assembler/boolean.h
	gcc -ansi -Wall -pedantic -c assembler/linked_list.c -o assembler/linked_list.o
