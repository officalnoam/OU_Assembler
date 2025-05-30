#ifndef FIRST_STAGE
  #include "linked_list.h"
  #include "operations.h"
  #include "boolean.h"

  #define FIRST_STAGE
  #define FIRST_STAGE_INPUT_SUFFIX ".am"
  #define BITS_IN_OPCODE 4
  #define BITS_IN_ADDRESSING 2
  #define BITS_IN_ARE 2
  #define BITS_IN_REG 4
  #define BITS_EXCLUDING_ARE (BITS_IN_BYTE - BITS_IN_ARE)

  /*
  This function will be used as a utility function when trying to parse and convert commands-
  by freeing the command, target and source arguments. If some don't exist- NULL should be passed instead.

  Input-
    char* command: The command string being freed.
    char* target: The target string being freed.
    char* source: The source string being freed.
  */
  void free_command_and_args(char* command, char* target, char* source);
  
  /*
  This function will check if a line has a symbol definition. It does not have to be a valid symbol.

  Input-
    char* line: The line being checked.
    char* file_name: The name of the file from which the line was read.
    int line_num: The number of the line within the file.
  Output-
    bool: true if the line contains a symbol definition- false otherwise.
  */
  bool has_symbol(char* line, char* file_name, int line_num);

  /*
  This function will create the byte of the command itself (as opposed to the command argument bytes).

  Input-
    operation op: The command being converted to binary.
    char* target: The tagret of the command.
    char* source: The source of the command.
    Node** head: A linked list that will contain the binary of the entire command.
    char* file_name: The file name the command was parsed from.
    int line_num: The line number within the file.
  */
  void create_command_byte(operation op, char* target, char* source, Node** head, char* file_name, int line_num);
  
  /*
  This function will create a byte for a singular argument.

  Input-
    char* arg: The argument.
    Node** head: The linked list that will contain the binary of the entire command.
    bool is_target: true if the argument is the target, false if it is the source.
  */
  void create_arg_byte(char* arg, Node** head, bool is_target);

  /*
  This function will create the bytes for both arguments (or the singular/none depending on the command).
  This might create a singular byte if both arguments are registers or if there is a singular argument.
  This might not create any bytes if there are no arguments.

  Input-
    char* target: The target argument.
    char* source: The source argument.
    Node** head: The linked list that will contain the binary of the entire command.
  */
  void create_arg_bytes(char* target, char* source, Node** head);

  /*
  Converts an entire command line into binary.

  Input-
    char* command: The command itself.
    char* line: The command line.
    int* i: A pointer to the index in the command line after the command.
    int *ic: A pointer to the instructions counter.
    char* file_name: The name of the file the command line was parsed from.
    int line_num: The number of the line within the file.

  Output-
    Node*: The byte(s) the command line was converted into.
  */
  Node* handle_command(char* command, char* line, int* i, int* ic, char* file_name, int line_num);

  /*
  Converts an entire directive line into binary.

  Input-
    char* directive: The directive itself.
    char* line: The directive line.
    int* i: A pointer to the index in the directive line after the directive.
    int* dc: A pointer to the data counter.
    char* file_name: The name of the file the directive line was parsed from.
    int line_num: The number of the line within the file.
    Node** externs: A linked list of the externs that will be defined within the file.
    Node** entries: A linked list of the entires that will be defined within the file.

  Output-
    Node*: The byte(s) the directive line was converted into.
  */
  Node* handle_directive(char* directive, char* line, int* i, int* dc, char* file_name, int line_num, Node** externs, Node** entries);
  
  /*
  Handles a symbol by verifying it's validity, adding it to the symbols list, and then handling the command/directive in the line.

  Input-
    char* line: The line containing the symbol.
    char* i: A pointer to the index within the line.
    Node** symbols_list: A linked list containing the symbols.
    Node* macro_list: A linked list containing the macros from the pre assembler stage. A symbol cannot have the same name as a macro.
    int* ic: A pointer to the instructions counter.
    int* dc: A pointer to the data counter.
    char* file_name: The file name the line was read from.
    int line_num: The number of the line within the file.
    Node** commands: A linked list containing the binary of the commands.
    Node** data_bin: A linked list containing the binary of the data. 
    Node** externs: A linked list of the externs that will be defined within the file.
    Node** entries: A linked list of the entires that will be defined within the file.
  */
  void handle_symbol(char* line, int* i, Node** symbols_list, Node* macro_list, int* ic, int* dc, char* file_name, int line_num, Node** commands, Node** data_bin, Node** externs, Node** entries);
  
  /*
  Handles the first stage of the assembler itself- reading the file, convering commands into binary and saving symbol strings
  and converting directives to data bytes. Also saving the defined entries and externals.

   Input-
    char* base_file: The base file name.
    Node* macro_list: A linked list containing the macros from the pre assembler stage. A symbol cannot have the same name as a macro.
    Node** symbols_list: A linked list containing the symbols.
    Node** commands: A linked list containing the binary of the commands.
    Node** data_bin: A linked list containing the binary of the data. 
    int* icf: A pointer to the instructions counter.
    int* dcf: A pointer to the data counter.
    Node** externs: A linked list of the externs that will be defined within the file.
    Node** entries: A linked list of the entires that will be defined within the file.
  */
  void first_stage(char* base_file, Node* macro_list, Node** symbols_list, Node** commands, Node** data_bin, int* icf, int* dcf, Node** externs, Node** entries);
#endif
