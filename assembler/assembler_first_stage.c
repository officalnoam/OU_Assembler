#include "assembler_first_stage.h" 
#include "arg_utils.h"
#include "string_utils.h"
#include "addressing.h"
#include "linked_list.h"
#include "registers.h"
#include "binary_utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void free_command_and_args(char* command, char* target, char* source)
{
  addressing source_addressing = get_addressing(source);
  addressing target_addressing = get_addressing(target);

  if (command != NULL)
    free(command);
  if (target != NULL && target_addressing != direct && target_addressing != matrix_access)
    free(target);
  if (source != NULL && source_addressing != direct && source_addressing != matrix_access)
    free(source);
}

bool has_symbol(char* line, char* file_name, int line_num)
{
  int i = 0;
  char* first_arg = get_argument(line, &i, line_num, file_name, false, false);
  
  if (first_arg[strlen(first_arg) - 1] == ':')
    return true;
  return false;
}

void create_command_byte(operation op, char* target, char* source, Node** head, char* file_name, int line_num)
{
  addressing source_addressing = get_addressing(source);
  addressing target_addressing = get_addressing(target);

  /*Since a byte can consist from up to four parts according to the assignment- create up to those parts.*/
  binary* total_bin = NULL;
  binary* bin1 = NULL; 
  binary* bin2 = NULL;
  binary* bin3 = NULL;
  binary* bin4 = NULL;

  binary_or_str* bos = NULL;

  /*Validate that the addressing types of the args makes sense*/
  if (!validate_args(op, target_addressing, source_addressing))
  {
    printf("Addressing types of arguments are not allowed for the command, in file %s line %d.\n", file_name, line_num);
    *head = NULL;
    return;
  }
  
  /*Creating the byte of the instruction*/
  bin1 = convert_num_to_binary(op - 2, BITS_IN_OPCODE, true);
  
  /*If the addressing for either source or target is undefined- manually pass
  0. Otherwise, the value for the binary is the same as the value for the addressing enum*/
  if (source_addressing == undefined_addressing)
    bin2 = convert_num_to_binary(0, BITS_IN_ADDRESSING, true);
  else
    bin2 = convert_num_to_binary(source_addressing, BITS_IN_ADDRESSING, true);
  
  if (target_addressing == undefined_addressing)
    bin3 = convert_num_to_binary(0, BITS_IN_ADDRESSING, true);
  else
    bin3 = convert_num_to_binary(target_addressing, BITS_IN_ADDRESSING, true);

  /*ARE of command is always 0*/
  bin4 = convert_num_to_binary(0, BITS_IN_ARE, true);
  
  total_bin = create_byte(bin1, bin2, bin3, bin4);
  free_multiple_binary(bin1, bin2, bin3, bin4);
    
  bos = create_binary_or_str(total_bin, NULL);
  *head = create_node(bos);
}

void create_arg_byte(char* arg, Node** head, bool is_target)
{
  addressing arg_addressing = get_addressing(arg);
  registers arg_reg = undefined_register;
  int* arg_int = NULL;
  Node* temp = NULL;
  
  /*The binary we deal with can have 3 parts*/
  binary* total_binary = NULL;
  binary* bin1 = NULL;
  binary* bin2 = NULL;
  binary* bin3 = NULL;

  char* mat_name = NULL;
  registers mat_reg_row = undefined_register;
  registers mat_reg_col = undefined_register;

  binary_or_str* bos = NULL;

  if (arg_addressing == immediate)
  {
    arg_int = convert_str_to_int(arg);

    /*Handle the arg not proeprly being an int by the format*/
    if (arg_int == NULL)
    {
      teardown_linked_list(*head, (void (*)(void *)) free_binary_or_str);
      *head = NULL;
      return;
    }

    bin1 = convert_num_to_binary(*arg_int, BITS_EXCLUDING_ARE, false);
    bin2 = convert_num_to_binary(0, BITS_IN_ARE, true);
    total_binary = create_byte(bin1, bin2, NULL, NULL);
    bos = create_binary_or_str(total_binary, NULL);
    temp = create_node(bos);
    add_node_to_end(*head, temp);
    free_multiple_binary(bin1, bin2, NULL, NULL);
    free(arg_int);
    return;
  }

  if (arg_addressing == register_addressing)
  {
    arg_reg = get_register(arg);
    bin1 = convert_num_to_binary(0, BITS_IN_REG, true);
    bin2 = convert_num_to_binary(arg_reg, BITS_IN_REG, true);
    bin3 = convert_num_to_binary(0, BITS_IN_ARE, true);
    if (is_target)
      total_binary = create_byte(bin2, bin1, bin3, NULL);
    else
      total_binary = create_byte(bin1, bin2, bin3, NULL);
    bos = create_binary_or_str(total_binary, NULL);
    temp = create_node(bos);
    add_node_to_end(*head, temp);
    free_multiple_binary(bin1, bin2, bin3, NULL);
    return;
  }

  /*We cannot currently convert to binary with direct addressing.
  Save the argument so that we can convert it in the second stage.*/
  if (arg_addressing == direct)
  {
    bos = create_binary_or_str(NULL, arg);
    temp = create_node(bos);
    add_node_to_end(*head, temp);
    return;
  }

  if (arg_addressing == matrix_access)
  {
    parse_matrix_argument(arg, &mat_name, &mat_reg_row, &mat_reg_col);

    if (mat_name == NULL)
    {
      teardown_linked_list(*head, (void (*)(void *)) free_binary_or_str);
      *head = NULL;
      return;
    }

    /*Cannot convert the mat name to binary so currently store it as is*/
    bos = create_binary_or_str(NULL, mat_name);
    temp = create_node(bos);
    add_node_to_end(*head, temp);

    bin1 = convert_num_to_binary(mat_reg_row, BITS_IN_REG, true);
    bin2 = convert_num_to_binary(mat_reg_col, BITS_IN_REG, true);
    bin3 = convert_num_to_binary(0, BITS_IN_ARE, true);
    total_binary = create_byte(bin1, bin2, bin3, NULL);
    bos = create_binary_or_str(total_binary, NULL);
    temp = create_node(bos);
    add_node_to_end(*head, temp);
    free_multiple_binary(bin1, bin2, bin3, NULL);
  }
}

void create_arg_bytes(char* target, char* source, Node** head)
{
  addressing source_addressing = get_addressing(source);
  addressing target_addressing = get_addressing(target);
  registers target_reg = undefined_register;
  registers source_reg = undefined_register;
  Node* temp = NULL;
  
  /*The binary we deal with can have 3 parts*/
  binary* total_binary = NULL;
  binary* bin1 = NULL;
  binary* bin2 = NULL;
  binary* bin3 = NULL;

  binary_or_str* bos = NULL;


  /*If target is undefined- both are, and no args exist so nothing needs to be done.*/
  if (target_addressing == undefined_addressing)
    return;
  
  /*Handle if both are regs*/
  if (target_addressing == register_addressing && source_addressing == register_addressing)
  {
    target_reg = get_register(target);
    source_reg = get_register(source);

    bin1 = convert_num_to_binary(source_reg, BITS_IN_REG, true);
    bin2 = convert_num_to_binary(target_reg, BITS_IN_REG, true);
    bin3 = convert_num_to_binary(0, BITS_IN_ARE, true);
    total_binary = create_byte(bin1, bin2, bin3, NULL);
    bos = create_binary_or_str(total_binary, NULL);
    temp = create_node(bos);
    add_node_to_end(*head, temp);
    free_multiple_binary(bin1, bin2, bin3, NULL);
    return;
  }
  
  create_arg_byte(source, head, true);

  if (*head == NULL || source == NULL)
    return;

  create_arg_byte(target, head, false);
}

Node* handle_command(char* command, char* line, int* i, int* dc, char* file_name, int line_num)
{
  operation op = get_operation(command);
  int arg_amount = get_command_arg_amount(op); 
  char* target = NULL;
  char* source = NULL;
  Node* head = NULL;
  Node* temp = NULL;
  int bytes = 0;

  /*Get either the source and target arguments or the target argument*/
  if (arg_amount == 2)
  {
    source = get_argument(line, i, line_num, file_name, false, false);
    
    if (source == NULL)
    {
      free_command_and_args(command, target, source);
      return NULL;
    }

    target = get_argument(line, i, line_num, file_name, true, true); 
    if (target == NULL)
    {
      free_command_and_args(command, target, source);
      return NULL;
    }
  }
  else if (arg_amount == 1)
  {
    source = get_argument(line, i, line_num, file_name, false, true);
    if (source == NULL)
    {
      free_command_and_args(command, target, source);
      return NULL;
    }
  }
  /*Make sure that if the command doesn't have arguments- check that there is no further text within the line*/
  else
  {
    push_index_to_non_whitespace(line, i);
    if (line[*i] != '\n' && line[*i] != '\0')
    {
      printf("Extratanious text in file %s line %d.\n", file_name, line_num);
      free_command_and_args(command, target, source);
      return NULL;
    }
  }

  create_command_byte(op, target, source, &head, file_name, line_num);

  if (head == NULL)
  {
    free_command_and_args(command, target, source);
    return NULL;
  }

  create_arg_bytes(target, source, &head);

  if (head != NULL)
  {
    temp = head;
    while (temp != NULL)
    {
      temp = temp->next;
      bytes++;
    }

    *dc = *dc + bytes;
  }
  
  /*If the arg creation had a problem, and head was torn down- NULL will be returned when returning head anyways.*/
  free_command_and_args(command, target, source);
  return head;  
}

/*
Node* handle_directive(char* directive, char* line, int* i, int* dc, char* file_name, int line_num)
{
  operation op = get_operation(directive);
}*/
/*
Node* handle_symbol(char* line, int* i, Node** symbols_list, Node* macro_list, int ic, int dc, char* file_name, int line_num)
{
  char* symbol_name = get_argument(line, i, line_num, file_name, false, false);

}

void first_stage(char* base_file, Node* macro_list, Node** symbols_list, Node** binary_lines, int* icf, int* dcf)
*/

#ifdef DEBUG_FIRST_STAGE
  int main()
  {
    int i = 4;
    int j;
    int dc = 0;
    Node* head = NULL;
    Node* temp = NULL;
    binary_or_str* tmp = NULL;
    char* command = (char*) malloc(4);

    strcpy(command, "add");

    printf("Should be 1 : %d. Should be 0 : %d.\n", has_symbol("Symbol: hello world", "my", 0), has_symbol("Nope no symbols here", "nope", 1));
    
    head = handle_command(command, "add #3, MAT[r2][r3]", &i, &dc, "hello world", 3);
    temp = head;
    while (temp != NULL)
    {
      tmp = (binary_or_str*) temp->data;
      if (tmp->str != NULL)
      {
        printf("%s\n", tmp->str);
      }
      else
      {
        for (j = 0; j < tmp->bin->size; j++)
        {
          printf("%d ", tmp->bin->bits[j]);
        }
        printf("\n");
      }
      temp = temp->next;
    }
    /*
    teardown_linked_list(head, (void (*)(void *)) free_binary_or_str);
    */
    return 0;
  }
#endif
