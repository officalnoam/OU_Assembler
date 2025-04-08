#include "operations.h"  
#include <string.h>
#ifdef DEBUG_OPERATIONS
  #include <stdio.h>
#endif

/*
Compare the string to all operation options- returning the matching option.
*/
operation get_operation(char* str)
{
  if (strcmp(str, "mcro") == 0)
    return mcro;
  if (strcmp(str, "mcroend") == 0)
    return mcroend;
  if (strcmp(str, "mov") == 0)
    return mov;
  if (strcmp(str, "cmp") == 0)
    return cmp;
  if (strcmp(str, "add") == 0)
    return add;
  if (strcmp(str, "sub") == 0)
    return sub;
  if (strcmp(str, "not") == 0)
    return not;
  if (strcmp(str, "clr") == 0)
    return clr;
  if (strcmp(str, "lea") == 0)
    return lea;
  if (strcmp(str, "inc") == 0)
    return inc;
  if (strcmp(str, "dec") == 0)
    return dec;
  if (strcmp(str, "jmp") == 0)
    return jmp;
  if (strcmp(str, "bne") == 0)
    return bne;
  if (strcmp(str, "red") == 0)
    return red;
  if (strcmp(str, "prn") == 0)
    return prn;
  if (strcmp(str, "jsr") == 0)
    return jsr;
  if (strcmp(str, "rts") == 0)
    return rts;
  if (strcmp(str, "stop") == 0)
    return stop;
  if (strcmp(str, ".data") == 0)
    return data;
  if (strcmp(str, ".string") == 0)
    return string;
  if (strcmp(str, ".mat") == 0)
    return mat;
  if (strcmp(str, ".entry") == 0)
    return entry;
  if (strcmp(str, ".extern") == 0)
    return extern_d;
  return undefined;
}

/*
For all the cases the operation isn't a command return false, by default in the switch case return true. 
*/
bool is_command(operation op)
{
  switch (op)
  {
    case mcro:
    case mcroend:
    case data:
    case string:
    case mat:
    case entry:
    case extern_d:
    case undefined:
      return false;
    default:
      return true;
  }
}

/*
If the operation is a directive return true, by default in the switch case return false.
*/
bool is_directive(operation op)
{
  switch (op)
  {
    case data:
    case string:
    case mat:
    case entry:
    case extern_d:
      return true;
    default:
      return false;
  }
}

/*
Remove 2 from the operation- since that is the opcode of the operation.
*/
int get_command_opcode(operation op)
{
  return op - 2;
}

/*
Debug main function for checking specifically the functions within the file. Get the operation of several strings, check if they are commands, directives etc.
*/
#ifdef DEBUG_OPERATIONS
  int main()
  {
    operation op = get_operation("Hello world");
    if (op == undefined)
      printf("Correct non operation parsing.\n");
    
    op = get_operation("mov");
    printf("mov- is_command: %d is_directive: %d opcode: %d\n", is_command(op), is_directive(op), get_command_opcode(op));

    op = get_operation(".data");
    printf(".data: is_comamnd %d is_directive %d\n", is_command(op), is_directive(op));
    return 0;
  }
#endif
