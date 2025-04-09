#include "string_utils.h"
  
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
