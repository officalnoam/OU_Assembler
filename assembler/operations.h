#ifndef COMMANDS
  #include "boolean.h"
  #include "addressing.h"
 
  #define COMMANDS
  
  /*Includes macro and endmacro, commands and directives. extern get an addition to the name due to it being a keyword in c*/
  typedef enum {
    mcro,
    mcroend,
    mov,
    cmp,
    add,
    sub,
    not,
    clr,
    lea,
    inc,
    dec,
    jmp,
    bne,
    red,
    prn,
    jsr,
    rts,
    stop,
    data,
    string,
    mat,
    entry,
    extern_d,
    undefined
  } operation;

  /*
  This function will return the enum option of the operation.
  
  Input-
    char* str: The string being converted to an enum option.
  Output-
    operation: The operation matching the string. If the string doesn't match any option- return undefined.
  */  
  operation get_operation(char* str);
  
  /*
  This function checks if the operation is a command.
   
  Input-
    operation op: The operation being checked.
  Output-
    bool: true if the operation is a command false otherwise.
  */
  bool is_command(operation op);

  /*
  This function checks if the operation is a directive (.string, .mat...)
  
  Input-
    operation op: The operation being checked.
  Output-
    bool: true if the operation is a directive, false otherwise.
  */
  bool is_directive(operation op);
  
  /*
  This function converts a command to its opcode.
  
  Input-
    operation op: The command being converted.
  Output-
    int: The commands opcode.
  */
  int get_command_opcode(operation op); 

  /*
  This function will validate the arguments, according to their addressing type.
  
  Input-
    operation op: The command.
    addressing* target: The addressing type for the target argument. NULL if there isn't one.
    addressing* source: The addressing type for the source argument. NULL if there isn't one.
  */ 
  bool validate_args(operation op, addressing target, addressing source);
  
  /*
  This function will return the amount of arguments the command has.
  
  Input-
    operation op: The command.
  Output-
    int: The amount of arguments the command should have.
  */
  int get_command_arg_amount(operation op);
#endif
