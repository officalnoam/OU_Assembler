#include "string_utils.h"
#include "ctype.h"
  
bool is_whitespace(char c)
{
  if (c == ' ' || c == '\t')
    return true;
  return false;
}

void push_index_to_non_whitespace(char* str, int* i)
{
  while (is_whitespace(str[*i]))
    *i = *i + 1;
}

/*
Will return if the rest of the string, starting from index i, contains an additional argument-
meaning a comma and then a number.
*/
bool str_contains_additional_num_arg(char* str, int i)
{
  /*Push the index passed the current number*/
  while (isdigit(str[i]) || str[i] == '-' || str[i] == '+')
    i++;

  /* Push until the end of the line or a comma*/
  while (str[i] != '\0' && str[i] != ',')
    i++;
    
  /*If there is no comma- there is no next argument*/
  if (str[i] != ',')
    return false;

  i++;

  while (str[i] != '\0')
  {
    if (isdigit(str[i]))
      return true;
    i++;
  }
  return false;
}