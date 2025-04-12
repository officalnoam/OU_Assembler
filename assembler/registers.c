#include "registers.h"
#include <string.h>
#ifdef DEBUG_REGISTERS
  #include <stdio.h>
#endif

/*
First check that the string is of length two, and starts with r.
Then check that the second character is between 0-7.
After these checks, we know the string is r{0-7}.
Based on the second character, retrun a specific register enum option.
*/
registers get_register(char* str)
{
  /*Make sure that the string starts with an r, and that the length of the string is two*/
  if (str[0] != 'r' || strlen(str) != 2)
    return undefined_register;
  
  /*Make sure that the second character is between 0-7*/
  if (str[1] < '0' || str[1] > '7')
    return undefined_register;
  
  return (registers) (str[1] - '0');
}

/*Debug main functions to check get_register*/
#ifdef DEBUG_REGISTERS
  int main()
  {
    printf("Should all be 1: %d %d %d %d %d\n", get_register("r0") == r0, get_register("r7") == r7, get_register("a2") == undefined_register, get_register("aaa") == undefined_register, get_register("r8") == undefined_register);
    return 0;
  }
#endif
