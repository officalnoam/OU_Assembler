#ifndef STRING_UTILS
  #include "boolean.h"
  
  #define STRING_UTILS

  /*
  This function checks if a character is a whitespace.
  
  Input-
    char c: The char being checked.
  Output-
    bool: true if the character is a whitespace, false otherwise.
  */  
  bool is_whitespace(char c);
  
  /*
  This function will push index i to the next non whitespace index in the string.
  
  Input-
    char* str: The string.
    int* i: The pointer to the index.
  */
  void push_index_to_non_whitespace(char* str, int* i);

  /*
  This function checks if there is an additional valid num argument in the string, starting from a specific index.

  Input-
    char* str: The string.
    int i: The index to start checking from.
  Output-
    bool: If the string contains a valid arg.
  */
  bool str_contains_additional_num_arg(char* str, int i);
#endif
