#ifndef FIRST_STAGE
  #include "linked_list.h"
  #include "operations.h"
  #include "boolean.h"

  #define FIRST_STAGE
  #define BITS_IN_OPCODE 4
  #define BITS_IN_ADDRESSING 2
  #define BITS_IN_ARE 2
  #define BITS_IN_REG 4
  #define BITS_EXCLUDING_ARE (BITS_IN_BYTE - BITS_IN_ARE)

  void free_command_and_args(char* command, char* target, char* source);  
  bool has_symbol(char* line, char* file_name, int line_num);
  void create_command_byte(operation op, char* target, char* source, Node** head, char* file_name, int line_num);
  void create_arg_byte(char* arg, Node** head, bool is_target);
  void create_arg_bytes(char* target, char* source, Node** head);
  Node* handle_command(char* command, char* line, int* i, int* ic, char* file_name, int line_num);
  Node* handle_directive(char* directive, char* line, int* i, int* dc, char* file_name, int line_num, Node** externs, Node** entries);
  Node* handle_symbol(char* line, int* i, Node** symbols_list, Node* macro_list, int ic, int dc, char* file_name, int line_num);

  void first_stage(char* base_file, Node* macro_list, Node** symbols_list, Node** binary_lines, int* icf, int* dcf);
#endif
