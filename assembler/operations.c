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
Depending on the operation type, check that the addressing types for the source and target are in ranges they should be.
*/
bool validate_args(operation op, addressing target, addressing source)
{
  switch (op)
  {
    case mov:
    case add:
    case sub:
    {
      if (target == undefined_addressing || source == undefined_addressing || target == immediate)
        return false;
      return true;
    }
    case cmp:
    {
      if (target == undefined_addressing || source == undefined_addressing)
        return false;
      return true;
    }
    case lea:
    {
      if (target == undefined_addressing || source == undefined_addressing || target == immediate || source == immediate || source == register_addressing)
        return false;
      return true;
    }  
    case not:
    case clr:
    case inc:
    case dec:
    case jmp:
    case bne:
    case red:
    case jsr:
      if (target == undefined_addressing || source != undefined_addressing || target == immediate)
        return false;
      return true;
    case prn:
      if (target == undefined_addressing || source != undefined_addressing)
        return false;
      return true;
    case rts:
    case stop:
      if (target != undefined_addressing || source != undefined_addressing)
        return false;
      return true;
    /*A non command was passed*/
    default:
      return false;
  }
}
/*Return the amount of arguments the command should have*/
int get_command_arg_amount(operation op)
{
  switch (op)
  {
    case mov:
    case add:
    case sub:
    case cmp:
    case lea:
      return 2;
    case not:
    case clr:
    case inc:
    case dec:
    case jmp:
    case bne:
    case red:
    case jsr:
    case prn:
      return 1;
    default:
      return 0;
  }
}

/*
Debug main function for checking specifically the functions within the file. Get the operation of several strings, check if they are commands, directives etc.
*/
#ifdef DEBUG_OPERATIONS
  int main()
  {
    operation op = get_operation("Hello world");
    addressing tmp1;
    addressing tmp2;
    if (op == undefined)
      printf("Correct non operation parsing.\n");
    
    op = get_operation("mov");
    printf("mov- is_command: %d is_directive: %d opcode: %d\n", is_command(op), is_directive(op), get_command_opcode(op));

    op = get_operation(".data");
    printf(".data: is_comamnd %d is_directive %d\n", is_command(op), is_directive(op));
    
    tmp1 = immediate;
    tmp2 = immediate;
    printf("Addressing validation checking- 1 : %d 0 : %d 1: %d 0: %d 1: %d\n", validate_args(cmp, tmp1, tmp2), validate_args(mov, tmp1, tmp2), validate_args(prn, tmp1, undefined_addressing), validate_args(prn, tmp1, tmp2), validate_args(stop, undefined_addressing, undefined_addressing));
    printf("Commands arg amount: 2 : %d 1 : %d 0 : %d\n", get_command_arg_amount(mov), get_command_arg_amount(not), get_command_arg_amount(stop));
    return 0;
  }
#endif
