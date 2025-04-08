#ifndef MACRO
  #include "linked_list.h"
  
  #define MACRO
  #define LINE_BUFFER 10

  typedef struct {
    char* name;
    char** lines;
    int line_amount;
    int line_capacity;
  } macro;

  /*
  This function will create a macro with the macro name.
  
  Input-
    char* macro_name: The name for the created macro.
  Output-
    macro*: A pointer to the created macro.
  */
  macro* create_macro(char* macro_name);

  /*
  This function will add an additional line to the lines field in the passed macro.
  
  Input=
    macro* mac: The pointer to the macro.
    char* line: The line being added.
  */
  void add_line_to_macro(macro* mac, char* line);

  /*
  This function will find a macro within a linked list of macros, based on the macro name.
  
  Input-
    Node* head: The pointer to the head of the linked list.
    char* macro_name: The name for the desired macro.
  Output-
    macro*: A pointer to the desired macro. If it's not in the linked list, return NULL.
  */
  macro* find_macro_in_list(Node* head, char* macro_name);
  
  /*
  Completely free the passed macro.
  
  Input-
    macro* mac: The macro being freed.
  */
  void free_macro(macro* mac);
#endif
