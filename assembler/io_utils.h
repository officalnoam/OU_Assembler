#ifndef IO_UTILS
  #include "boolean.h"
  #include <stdio.h>
  
  #define IO_UTILS
  #define LINE_LEN 82
  
  /*
  This function will find the current line number of the position within the file.
  
  Input-
    FILE* file: The file being checked.
  Output-
    int: The current line number in the file.
  */
  int get_current_line_num(FILE* file);  
  
  /*
  This function will get a single line from the file. If the file has reached EOF- return NULL.
  
  Input-
    FILE* file: The file being read from.
    char* file_name: The name of the file being read from. Will be used if needed for error messages.
    bool* error_reached: If an error has been reached during the entire process. If any line is too line- change the error_reached to true.
  Output-
    char*: The line read from the file.
  */
  char* get_file_line(FILE* file, char* file_name, bool* error_reached);
  
  /*
  This function will write a single line from the file.
  
  Input-
    FILE* file: The file being written to.
    char* file_line: The line being witten. 
  */
  void write_file_line(FILE* file, char* file_line);
#endif
