#include "pre_assembler.h"
#include "macro.h"
#include "io_utils.h"
#include "operations.h"
#include "arg_utils.h"
#include "file_utils.h"
#include <stdlib.h> 

/*
Free/fclose/teardown to all passed variable to all variabeles that are not NULL (so they need to be freed).
*/
void teardown_all(FILE* input_file, FILE* output_file, char* input_file_name, char* output_file_name, Node* head)
{
  if (input_file != NULL)
    fclose(input_file);
  if (output_file != NULL)
    fclose(output_file);
  if (input_file_name != NULL) 
    free(input_file_name);
  if (output_file_name != NULL)
    free(output_file_name);
  if (head != NULL)
    teardown_linked_list(head, (void (*)(void *)) free_macro);    
}

/*
Check that the macro name is fine- not a reserved word in assembly or an already existing macro.
Add all lines until the mcroend command to the macro being created.
Enforce the syntax of specifically the mcroend comamnd.
Add the macro to the linked list of the macros.
*/
void handle_macro(FILE* input_file, Node** head, char* macro_name, char* file_name, int* line_number, bool* error_reached)
{
  int i = 0;
  char* line = NULL;
  char* arg = NULL;
  operation op;
  macro* mac = NULL;
  Node* new_node = NULL;
 
  /*Validate macro name*/

  /*Check that the macro name isn't equal to a defined operation*/
  op = get_operation(macro_name);
  if (op != undefined)
  {
    printf("Error in file %s line %d: macro name '%s' is name of operation in assembly", file_name, *line_number, macro_name);
    *error_reached = true;
    return;
  }
  
  /*Check that the macro name isn't equal to an already defined macro name*/
  mac = find_macro_in_list(*head, macro_name);
  if (mac != NULL)
  {
    printf("Error in file %s line %d: macro name '%s' is a name of an already defined macro", file_name, *line_number, macro_name);
    *error_reached = true;
    return;
  }
   
  mac = create_macro(macro_name);  
 
  line = get_file_line(input_file, file_name, error_reached);
  while (line != NULL && !*error_reached)
  {
    i = 0;
    *line_number = *line_number + 1;

    if (is_line_comment(line) || is_line_whitespaces(line, &i))
    {
      add_line_to_macro(mac, line);
      continue;
    }
    arg = get_argument(line, &i, *line_number, file_name, false, false);
    
    /*If the arg is equal to NULL- there is a problem when parsing an argument within the line, but it doesn't matter
    pre_assembler doesn't handle it.*/
    if (arg == NULL)
    {
      add_line_to_macro(mac, line);
      continue;
    }

    op = get_operation(arg);    
    free(arg);
    
    /*Handle reaching the macro end operation*/
    if (op == mcroend)
    {
      i = 0;
      /*Check that the mcroend line syntax is fine.*/
      arg = get_argument(line, &i, *line_number, file_name, false, true);
      if (arg == NULL)
      {
        *error_reached = true;
        free(line);
        free_macro(mac);
        return;
      }
      
      /*Free up arg and line (since the line won't be added to the macro). End finding the lines within the macro*/
      free(arg);
      free(line);
      break;
    }
    add_line_to_macro(mac, line);
    line = get_file_line(input_file, file_name, error_reached);
  }
  
  new_node = create_node(mac); 
  /*Add the created macro to the linked list*/
  if (*head != NULL)
    add_node_to_end(*head, new_node);   
  /*If the head is NULL- meaning that the created macro is the first, set the head to the created macro.*/
  else
    *head = new_node;
}

/*
Open the input and output files.
If the file is a comment or whitespace line- automatically write it in the output file.
If the line is calling a defined macro- do not write the current line and instead write all of the lines in the defined macro.
If the line is defining a macro- pass the handling to handle_macro
Otherwise, write the line in the output file.
*/
Node* pre_assembler(char* base_file, bool* error_reached)
{
  int i = 0;
  int j;
  int line_number = 0;
  FILE* input_file = NULL;
  FILE* output_file = NULL;
  char* input_file_name = create_full_file_name(base_file, INPUT_SUFFIX);
  char* output_file_name = create_full_file_name(base_file, OUTPUT_SUFFIX);
  char* arg = NULL;
  char* line = NULL;
  operation op;
  Node* head = NULL;
  macro* mac = NULL;

  /*Handle memory allocation error with file names*/
  if (input_file_name == NULL || output_file_name == NULL)
  {
    teardown_all(input_file, output_file, input_file_name, output_file_name, head);
    return NULL;
  }

  /*Open files*/
  open_files(&input_file, &output_file, input_file_name, output_file_name);
  
  /*Handle file opening error*/
  if (input_file == NULL || output_file == NULL)
  {
    teardown_all(input_file, output_file, input_file_name, output_file_name, head);
    return NULL;
  }  
  
  while ((line = get_file_line(input_file, input_file_name, error_reached)) && !*error_reached)
  {
    i = 0;
    line_number++;
    
    /*Handle comment or whitespaces lines*/
    if (is_line_comment(line) || is_line_whitespaces(line, &i))
    {
      write_file_line(output_file, line);
      free(line);
      continue;
    }
    
    arg = get_argument(line, &i, line_number, input_file_name, false, false);
    /*If arg failed to be parsed - doesn't matter, since the pre_assembler doesn't handle it*/
    if (arg == NULL)
    {
      write_file_line(output_file, line);
      free(line);
      continue;
    } 
    
    op = get_operation(arg);
    mac = find_macro_in_list(head, arg);
    free(arg);
    
    /*Check if the argument is an already defined macro. If it is, write the macro lines into the file instead of the macro call.*/
    if (mac != NULL)
    {
      for (j = 0; j < mac->line_amount; j++)
        write_file_line(output_file, mac->lines[j]);
      free(line);
      continue;
    }
    
    /*Handle if the line is a macro definition.*/
    if (op == mcro)
    {
      arg = get_argument(line, &i, line_number, input_file_name, false, true);
      
      /*If arg is NULL the macro name has a syntax problem.*/
      if (arg == NULL || arg[0] == '\0')
      {
        *error_reached = true;
        free(line);
        teardown_all(input_file, output_file, input_file_name, output_file_name, head);
        return NULL; 
      }
      
      handle_macro(input_file, &head, arg, input_file_name, &line_number, error_reached);
      continue;
    }
    
    write_file_line(output_file, line);
    free(line);
  }
   
  /*Do not free the macro linked list- the pre assembler was successful so we want to return it*/
  teardown_all(input_file, output_file, input_file_name, output_file_name, NULL);
  return head;
}
