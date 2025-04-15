#ifndef PRE_ASSEMBLER
  #include "linked_list.h"
  #include "boolean.h"
  #include <stdio.h>
  
  #define PRE_ASSEMBLER
  #define SUFFIX_SIZE 3
  #define INPUT_SUFFIX ".as"
  #define OUTPUT_SUFFIX ".am"
  
  /*
  This function will create a full file name from the base file name and a suffix.
  
  Input-
    char* base_file_name: The base file name.
    char* suffix: The suffix being added.
  Output-
    char*: The full file name.
  */
  char* create_full_file_name(char* base_file_name, char* suffix);
  
  /*
  This function will open an input and output file.
  
  Input-
    FILE** input_file: The input file.
    FILE** output_file: The output file.
    char* input_file_name: The name of the input file.
    char* output_file_name: The name of the output file.
  */
  void open_files(FILE** input_file, FILE** output_file, char* input_file_name, char* output_file_name);
  
  /*
  This function will teardown all of the things that need to be closed in the pre assembler. If they are NULL- they won't be freed.
  
  Input-
    FILE* input_file: The input file.
    FILE* output_file: The output file.
    char* input_file_name: The input file name.
    char* output_file_name: The output file name.
    Node* head: The head of the linked list containing all of the macros.
  */
  void teardown_all(FILE* input_file, FILE* output_file, char* input_file_name, char* output_file_name, Node* head);

  /*
  This function will handle the definition of a macro within the input file.
  
  Input-
    FILE* input_file: The input file.
    Node** head: The head of the linked list of the macros. If the linked list doesn't contain any nodes yet- the defined macro will be the new head.
    char* macro_name: The name of the newly creared macro.
    char* file_name: The name of the input file. Used for error printing.
    int* line_number: The line of the current number being read. Used for error printing.
    bool* error_reached: Signifies if an error was reached during the pre assembler so far. If an error is reached is set to true.
  */
  void handle_macro(FILE* input_file, Node** head, char* macro_name, char* file_name, int* line_number, bool* error_reached);
  
  /*
  This function will completly run the pre assembler on the file.
  
  Input-
    char* base_file: The base name of the file the pre assembler is run on.
    bool* error_reached: Signifies if an error is reached during the pre assembler duration of the file. If error is reached- set to true, and further
			 assembler stages won't be ran.
  Output-
    Node*: The head of the macro linked list. This will be used to check that the symbols don't have the same name as a macro.
  */
  Node* pre_assembler(char* base_file, bool* error_reached);
#endif
