#include "arg_utils.h"
#include "string_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

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
  
  /*If j is 0- there isn't an argument.*/
  if (j == 0)
  {
    printf("Missing argument in file %s line %d.\n", file_name, line_num);
    free(str);
    return NULL;
  }
  str[j] = '\0';
  return str;
}

/*
Move passed the # if it exists, check if the number is a minus or not- and validate the string is a num.
Otherwise NULL will be returned.
*/
int* convert_str_to_int(char* str)
{
  int i = 0;
  bool is_negative = false;
  int* actual_num = NULL;

  actual_num = (int*) malloc(sizeof(int));

  if (actual_num == NULL)
  {
    printf("Memory allocation failure.\n");
    return NULL;
  }

  *actual_num = 0;

  if (str[i] == '#')
    i++;

  if (str[i] == '-')
  {
    is_negative = true;
    i++;
  }
  else if (str[i] == '+')
    i++;
  
  /*Convert string to the num*/
  for (; i < strlen(str); i++)
  {
    if (!isdigit(str[i]))
    {
      printf("Inputted number %s contains non digit characters.\n", str);
      free(actual_num);
      return NULL;
    }
    *actual_num = *actual_num * 10 + (int) (str[i] - '0');
  }

  if (is_negative)
    *actual_num = *actual_num * -1;

  return actual_num;
}

/*
Will go over the string, will copy it until reaching []s, and parse the registers within them.
*/
void parse_matrix_argument(char* str, char** mat_name, registers* mat_reg_row, registers* mat_reg_col)
{
  int i = 0;
  int j = 0;
  char temp[2];
  
  *mat_name = (char*) malloc(sizeof(char) * ARG_LEN);

  if (*mat_name == NULL)
  {
    printf("Memory allocation failure.\n");
    return;
  }

  while (str[i] != '\0' && str[i] != '\n' && str[i] != '[')
  {
    (*mat_name)[j] = str[i];
    i++;
    j++;
  }

  (*mat_name)[j] = '\0';

  if (strlen(str) != i + 8 || str[i] != '[' || str[i + 3] != ']' || str[i + 4] != '[' || str[i + 7] != ']')
  {
    printf("Inputted matrix arg isn't in proper format.\n");
    free(*mat_name);
    *mat_name = NULL;
    return;
  }

  temp[0] = str[i + 1];
  temp[1] = str[i + 2];
  *mat_reg_row = get_register(temp);
  
  temp[0] = str[i + 5];
  temp[1] = str[i + 6];
  *mat_reg_col = get_register(temp);
  
  if (*mat_reg_row == undefined_register || *mat_reg_col == undefined_register)
  {
    printf("Inputted matrix registers aren't actually registers.\n");
    free(*mat_name);
    *mat_name = NULL;
    return;
  }

}

Node* parse_int_list(char* line, int* i, int line_num, char* file_name)
{
  Node* head = NULL;
  Node* temp = NULL;
  int* parsed_int = NULL;
  char* str_num = NULL;
  bool starts_with_comma = false;
  bool not_last_arg = true;

  while (not_last_arg)
  {
    not_last_arg = str_contains_additional_num_arg(line, *i + 1);
    str_num = get_argument(line, i, line_num, file_name, starts_with_comma, !not_last_arg);

    starts_with_comma = true;
    
    if (str_num == NULL)
    {
      teardown_linked_list(head, free);
      return NULL;
    }

    parsed_int = convert_str_to_int(str_num);
    free(str_num);

    if (parsed_int == NULL)
    {
      teardown_linked_list(head, free);
      return NULL;
    }

    temp = create_node(parsed_int);

    if (head == NULL)
      head = temp;
    else
      add_node_to_end(head, temp);    
  }

  return head;
}

int* parse_matrix_definition(char* line, int* i, int line_num, char* file_name, Node** parsed_int_list)
{
  int j;
  int temp = 0;
  char* arg = get_argument(line, i, line_num, file_name, false, false);
  int* matrix_items = NULL; 

  if (arg == NULL)
    return NULL;

  if (arg[0] != '[' || arg[strlen(arg) - 1] != ']')
  {
    printf("Incorrect format for matrix definition in %d line of %s file\n", line_num, file_name);
    free(arg);
    return NULL;
  }

  matrix_items = (int*) malloc(sizeof(int));
  
  if (matrix_items == NULL)
  {
    perror("Memory allocation failure.\n");
    free(arg);
    return NULL;
  }

  *matrix_items = 0;

  /*Parse the row and column definition. Multiply both to get the matrix items*/
  for (j = 1; j < strlen(arg) - 1; j++)
  {
    if (isdigit(arg[j]))
      temp = temp * 10 + (arg[j] - '0');

    /*Handle the seperation between the row and column amounts*/
    else if (arg[j] == ']')
    {
      j++;
      if (arg[j] != '[')
      {
        printf("Incorrect format for matrix definition in %d line of %s file\n", line_num, file_name);
        free(arg);
        free(matrix_items);
        return NULL;
      }
      *matrix_items = temp;
      temp = 0;
    }
    else
    {
      printf("Incorrect format for matrix definition in %d line of %s file\n", line_num, file_name);
      free(arg);
      free(matrix_items);
      return NULL;
    }
  }

  /*If the matrix items is still 0 - then there was only one square bracket and not two.*/
  if (*matrix_items == 0)
  {
    printf("Incorrect format for matrix definition in %d line of %s file\n", line_num, file_name);
    free(arg);
    free(matrix_items);
    return NULL;
  }

  *matrix_items = *matrix_items * temp;

  /*If there is nothing left for the rest of the line- no need to parse an int list*/
  if (is_line_whitespaces(line, i))
    return matrix_items;

  *parsed_int_list = parse_int_list(line, i, line_num, file_name);

  /*Handle failure with int list*/
  if (*parsed_int_list == NULL)
  {
    free(arg);
    free(matrix_items);
    return NULL;
  }

  return matrix_items;
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
    int* tmp2 = NULL;
    char* mat_name = NULL;
    registers reg_row = undefined_register;
    registers reg_col = undefined_register;
    Node* head = NULL;
    Node* temp = NULL;

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

    tmp2 = convert_str_to_int("1");
    printf("Should be 1: %d\n", *tmp2 == 1);
    free(tmp2);
    tmp2 = convert_str_to_int("#+1");
    printf("Should be 1: %d\n", *tmp2 == 1);
    free(tmp2);
    tmp2 = convert_str_to_int("-1");
    printf("Should be 1: %d\n", *tmp2 == -1);
    free(tmp2);

    parse_matrix_argument("MAT[r2][r3]", &mat_name, &reg_row, &reg_col);
    printf("%s Should be 1: %d %d\n", mat_name, reg_row == r2, reg_col == r3);
    free(mat_name);
    mat_name = NULL;
    parse_matrix_argument("MAT[][r3]", &mat_name, &reg_row, &reg_col);
    printf("Should be 1: %d\n", mat_name == NULL);

    i = 0;
    head = parse_int_list("1, 2, 3,4,  5", &i, 1, "hello");
    temp = head;
    while (temp != NULL)
    {
      printf("%d ", *(int*)(temp->data));
      temp = temp->next;
    }
    printf("\n");

    teardown_linked_list(head, free);

    i = 4;
    tmp2 =  parse_matrix_definition(".mat [2][3] +1, -3, 2", &i, 1, "hello", &head);
    printf("Should be 1: %d\n", *tmp2 == 6);
    free(tmp2);

    temp = head;
    while (temp != NULL)
    {
      printf("%d ", *(int*)(temp->data));
      temp = temp->next;
    }

    printf("\n");

    teardown_linked_list(head, free);

    return 0;
  }
#endif
