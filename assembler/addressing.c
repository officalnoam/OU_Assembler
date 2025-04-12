#include "addressing.h"
#include "registers.h"
#include <string.h>
#ifdef DEBUG_ADDRESSING
  #include <stdio.h>
#endif

/*
Check for signifiers of each addressing option and return the matching option.
*/
addressing get_addressing(char* str)
{
  int i;
  registers reg;

  /*The first character being a # signifies the string being immediate addressing.*/  
  if (str[0] == '#')
    return immediate;

  /*If a proper register is returned from get_register- it means that the string is a register*/
  reg = get_register(str);
  if (reg != undefined_register)
    return register_addressing;
  
  /*A bracket in the string signifies matrix access*/
  for (i = 0; i < strlen(str); i++)
  {
    if (str[i] == '[' || str[i] == ']')
      return matrix_access;
  }
  
  /*Last option is direct access*/
  return direct;
}

/*Debug main function to check get_addressing*/
#ifdef DEBUG_ADDRESSING
  int main()
  {
    printf("Should only be 1s: %d %d %d %d\n", get_addressing("r0") == register_addressing, get_addressing("#-3") == immediate, get_addressing("MAT[1][2]") == matrix_access, get_addressing("x") == direct);
    return 0;
  }
#endif
