#ifndef ARG_UTILS
  #include "boolean.h"

  #define ARG_UTILS
  #define ARG_LEN 81

  /*
  This function checks if the line is a comment. This is signified if the first character of the line is a ';'.
  
  Input-
    char* line: The line being checked.
  Output-
    bool: true if the line is a comment, false otherwise.
  */
  bool is_line_comment(char* line);
  
  /*
  This function checks if the line only contains whitespaces.
  
  Input-
    char* line: The line being checked.
    int* i: A pointer to the index in the line from which the line will be checked.
  Output-
    bool: true if the line is only whitespaces, false otherwise.
  */
  bool is_line_whitespaces(char* line, int* i);
  
  /*
  This function gets an argument from a certain position in a line, enforcing correct syntax.
  
  Input-
    char* line: The line the argument is in.
    int* i: The pointer to the index from which the argument parsing begins.
    int line_num: The number of the line within the file. Used for error printing.
    char* file_name: The name of the file the line is in. Used for error printing.
    bool starts_with_comma: true if a comma needs to appear before the argument, false otherwise.
    bool last_argument: true if there shouldn't be any non whitespaces after the argument, false otherwise.
  Output-
    char*: The parsed argument. NULL if there is a syntax error within the line. 
  */
  char* get_argument(char* line, int* i, int line_num, char* file_name, bool starts_with_comma, bool last_argument);
  
  /*
  This function will convert the string of an int into an integer.

  Input-
    char* str: The string of the int.asm
  Output-
    int*: The int.
  */
  int* convert_str_to_int(char* str);
#endif
