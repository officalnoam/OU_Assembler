#include "arg_utils.h"
#include "string_utils.h"
#include <stdlib.h>
#include <stdio.h>

/*
Check if the line is a comment by comparing the first char in the line to ';'
*/  
bool is_line_comment(char* line)
{
  return line[0] == ';';
}

/*
Check if the line is only whitespaces by pushing the index of the line to a non whitespace character- 
and checking if the end of the string was reached.
*/
bool is_line_whitespaces(char* line, int* i)
{ 
  push_index_to_non_whitespace(line, i);
  if (line[*i] == '\n' || line[*i] == '\0')
    return true;
  return false;
}

/*
First push the index to a non whitespace.
Then enforce comma syntax if needed, and further push the index to the argument itself.
Copy the argument until a whitespace or the end of the line is reached.
If relevant, enforce last argument syntax.
*/
char* get_argument(char* line, int* i, int line_num, char* file_name, bool starts_with_comma, bool last_argument)
{
  char* str = NULL;
  int j = 0;
  
  /*Ignore whitespaces before argument*/
  push_index_to_non_whitespace(line, i);
  
  /*If a comma should exist before the argument- make sure the first char after the whitespaces is the comma*/
  if (starts_with_comma)
  {
    if (line[*i] != ',')
    {
      printf("Missing comma in file %s line %d.\n", file_name, line_num);
      return NULL;
    }
    *i = *i + 1;
  }
  
  /*Ignore all whitespaces between comma (if it exists) and argument*/
  push_index_to_non_whitespace(line, i);
  
  /*Make sure there isn't a comma when there shouldn't be one.*/
  if (line[*i] == ',')
  {
    printf("Unneeded comma in file %s line %d.\n", file_name, line_num);
    return NULL;
  }
  
  str = (char*) malloc(sizeof(char) * ARG_LEN);

  if (str == NULL)
  {
    perror("Memory allocation failed.\n");
    return NULL;
  }
  
  /*Get the argument until a whitespace, comma or the end of the string is reached*/
  while (!is_whitespace(line[*i]) && line[*i] != '\n' && line[*i] != '\0' && line[*i] != ',') 
  {
    str[j] = line[*i];
    *i = *i + 1;
    j++;
  }
  
  /*If the argument is supposed to be the last argument- make sure no other non whitespaces exist until the end of the line*/
  if (last_argument)
  {
    push_index_to_non_whitespace(line, i);
    if (line[*i] != '\n' && line[*i] != '\0')
    {
      printf("Extratanious text in file %s line %d.\n", file_name, line_num);
      free(str);
      return NULL;
    }
  }
  str[j] = '\0';
  return str;
}

/*
This is a debug main function, used to check the functions within this file.
*/
#ifdef DEBUG_ARGS
  int main()
  {
    int i = 0;
    char* str = "hello  world :)";
    char* str2 = "\t\t\t   \t\n";
    char* str3 = ";This is a comment!!!!\n";
    char* tmp = NULL;
    printf("Is whitespace line (NO)? %d Comment (NO)? %d\n", is_line_whitespaces(str, &i), is_line_comment(str));
    i = 0;
    printf("Is whitespace line (YES)? %d Comment (YES)? %d\n", is_line_whitespaces(str2, &i), is_line_comment(str3));
    i = 0;   
 
    tmp = get_argument(str, &i, 1, "temp.txt", false, false);
    printf("First arg: %s\n", tmp);
    free(tmp);
    tmp = NULL;
    tmp = get_argument(str, &i, 1, "temp.txt", false, false);
    printf("Second arg: %s\n", tmp);
    free(tmp);
    tmp = NULL;
    tmp = get_argument(str, &i, 1, "temp.txt", false, true);
    printf("Third arg: %s\n", tmp);
    free(tmp);
    tmp = NULL;

    i = 0;
    tmp = get_argument(str, &i, 1, "temp.txt", true, false);
    if (tmp != NULL)
      printf("Failed to recognize missing comma");
    tmp = get_argument(str, &i, 1, "temp.txt", false, true);
    if (tmp != NULL)
      printf("Failed to recognize bad ending syntax");

    str = "hello, world";
    i = 0;
    tmp = get_argument(str, &i, 1, "temp.txt", false, false);
    printf("First arg %s\n", tmp);
    free(tmp);
    tmp = NULL;
    tmp = get_argument(str, &i, 1, "temp.txt", true, true);
    printf("Second arg %s\n", tmp);
    free(tmp);
    return 0;
  }
#endif
