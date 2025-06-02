#ifndef ADDRESSING
  #define ADDRESSING  

  typedef enum {
    immediate,
    direct,
    matrix_access,
    register_addressing,
    undefined_addressing
  } addressing;

  /*
  This function will get the addressing type of the passed string.
  
  Input-
    char* str: The string.
  Output-
    addressing: The addressing type.
  */  
  addressing get_addressing(char* str);

  /*
  This function will get the amount of bytes the addressing type creates.

  Input-
    addressing address: The addressing type of the arg.
  Output-
    int: The amount of bytes created for the addressing type.
  */
  int get_addressing_bytes(addressing address);
#endif
