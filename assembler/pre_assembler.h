#ifndef PRE_ASSEMBLER
  #include "linked_list.h"
  #include "boolean.h"
  #include <stdio.h>
  
  #define PRE_ASSEMBLER
  #define SUFFIX_SIZE 3
  #define INPUT_SUFFIX ".as"
  #define OUTPUT_SUFFIX ".am"

  char* create_full_file_name(char* base_file_name, char* suffix);
  void open_files(FILE** input_file, FILE** output_file, char* input_file_name, char* output_file_name);
  void teardown_all(FILE* input_file, FILE* output_file, char* input_file_name, char* output_file_name, Node* head);
  void handle_macro(FILE* input_file, Node** head, char* macro_name, char* file_name, int* line_number, bool* error_reached);
  void pre_assembler(char* base_file, bool* error_reached);
#endif
