#ifndef REGISTERS
  #define REGISTERS

  typedef enum {
    r0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7,
    undefined_register
  } registers;

  /*
  This function will convert a string into the matching register.
  
  Input-
    char* str: The string.
  Output-
    registers: The register matching the string.
  */  
  registers get_register(char* str);
#endif
