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
#endif
