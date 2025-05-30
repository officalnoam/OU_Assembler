#include "assembler_first_stage.h" 
#include "arg_utils.h"
#include "string_utils.h"
#include "addressing.h"
#include "linked_list.h"
#include "registers.h"
#include "binary_utils.h"
#include "symbol.h"
#include "macro.h"
#include "file_utils.h"
#include "io_utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/*
Free the command, target and source if they aren't NULL.
*/
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

/*
Check if the first argument in the line is a symbol or not according to the symbol syntax (:).
*/
bool has_symbol(char* line, char* file_name, int line_num)
{
  int i = 0;
  char* first_arg = get_argument(line, &i, line_num, file_name, false, false);
  
  if (first_arg[strlen(first_arg) - 1] == ':')
    return true;
  return false;
}

/*
Convert the opcode of the command to binary, and then convert the addressing of the arguments to binary,
Create from those binaries a command byte.
*/
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

/*
Convert the argument to a byte, based on the addressing of the argument.
*/
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

/*
Create the bytes for both arguments (if they are not NULL).
*/
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

/*
Conver the entire command to bytes.
*/
Node* handle_command(char* command, char* line, int* i, int* ic, char* file_name, int line_num)
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

    *ic = *ic + bytes;
  }
  
  /*If the arg creation had a problem, and head was torn down- NULL will be returned when returning head anyways.*/
  free_command_and_args(command, target, source);
  return head;  
}

/*
Convert a directive to bytes.
*/
Node* handle_directive(char* directive, char* line, int* i, int* dc, char* file_name, int line_num, Node** externs, Node** entries)
{
  int j;
  operation op = get_operation(directive);
  char* arg = NULL;
  char* symbol = NULL;
  Node* head = NULL;
  Node* temp = NULL;
  Node* int_list_head = NULL;
  Node* int_list_temp = NULL;
  int* matrix_size = NULL;
  int temp_value = 0;
  binary* byte = NULL;

  switch (op)
  {
    case data:
      int_list_head = parse_int_list(line, i, line_num, file_name);
      
      /*Handle failure in parsing the list*/
      if (int_list_head == NULL)
      {
        free(directive);
        return NULL;
      }

      /*Convert each int to a byte*/
      int_list_temp = int_list_head;
      while (int_list_temp != NULL)
      {
        byte = convert_num_to_binary(*(int*)(int_list_temp->data), BITS_IN_BYTE, false);

        temp = create_node(byte);
        if (head == NULL)
          head = temp;
        else
          add_node_to_end(head, temp);

        *dc = *dc + 1;
        int_list_temp = int_list_temp->next;
      }

      teardown_linked_list(int_list_head, free);
      break;

    case string:
      arg = get_argument(line, i, line_num, file_name, false, true);

      /*Check that get_argument worked*/
      if (arg == NULL)
      {
        free(directive);
        return NULL;
      }
      
      /*Handle the string not being the proper format*/
      if (strlen(arg) < 3 || arg[0] != '"' || arg[strlen(arg) - 1] != '"')
      {
        printf("String directive - %s in line %d file %s filename is improperly formatted", arg, line_num, file_name);
        free(arg);
        free(directive);
        return NULL;
      }

      /*Convert each character to a byte.*/
      for (j = 1; j < strlen(arg) - 1; j++)
      {
        byte = convert_num_to_binary((int) arg[j], BITS_IN_BYTE , false);
        
        temp = create_node(byte);
        if (head == NULL)
          head = temp;
        else
          add_node_to_end(head, temp);    
      }
      byte = convert_num_to_binary(0, BITS_IN_BYTE, true);
      temp->next = create_node(byte);
      *dc = *dc + strlen(arg) - 1;
      free(arg);

      break;
      
    case mat:
      matrix_size = parse_matrix_definition(line, i, line_num, file_name, &int_list_head);
      /*Handle the matrix argument not being a valid format*/
      if (matrix_size == NULL)
      {
        free(directive);
        return NULL;
      }

      int_list_temp = int_list_head;
      for (j = 0; j < *matrix_size; j++)
      {
        temp_value = 0;
        if (int_list_temp != NULL)
        {
          temp_value = *(int*)(int_list_temp->data);
          int_list_temp = int_list_temp->next;
        }
        byte = convert_num_to_binary(temp_value, BITS_IN_BYTE, false);
        
        temp = create_node(byte);
        if (head == NULL)
          head = temp;
        else
          add_node_to_end(head, temp);
        
        *dc = *dc + 1;
      }

      free(matrix_size);
      teardown_linked_list(int_list_head, free);
      break;

    case entry:
      symbol = get_argument(line, i, line_num, file_name, false, true);

      if (symbol == NULL)
      {
        free(directive);
        return NULL;
      }

      temp = create_node(symbol);

      if (*entries == NULL)
        *entries = temp;
      else
        add_node_to_end(*entries, temp);
      break;
    
    case extern_d:
      symbol = get_argument(line, i, line_num, file_name, false, true);

      if (symbol == NULL)
      {
        free(directive);
        return NULL;
      }

      temp = create_node(symbol);

      if (*externs == NULL)
        *externs = temp;
      else
        add_node_to_end(*externs, temp);
      break;
    
    default:
      return NULL;
  }

  free(directive);
  return head;
}
void handle_symbol(char* line, int* i, Node** symbols_list, Node* macro_list, int* ic, int* dc, char* file_name, int line_num, Node** commands, Node** data_bin, Node** externs, Node** entries)
{
  char* symbol_name = get_argument(line, i, line_num, file_name, false, false);
  char* arg = NULL;
  operation op;
  symbol* sym = NULL;
  Node* sym_node = NULL;
  Node* binary = NULL;
  
  symbol_name[strlen(symbol_name) - 1] = '\0';

  if (get_operation(symbol_name) != undefined || strlen(symbol_name) > 30 || !isalpha(symbol_name[0]) || find_macro_in_list(macro_list, symbol_name) != NULL || find_symbol_in_list(*symbols_list, symbol_name))
  {
    printf("Symbol name %s in file %s line %d is invalid. Will not save symbol.\n", symbol_name, file_name, line_num);
    free(symbol_name);
    symbol_name = NULL;
  }

  arg = get_argument(line, i, line_num, file_name, false, false);

  if (arg == NULL)
  {
    free(symbol_name);
    return;
  }

  op = get_operation(arg);

  if (symbol_name != NULL)
  {
    sym = (symbol*) malloc(sizeof(symbol));

    if (sym == NULL)
    {
      perror("Memory allocation failure.\n");
      return;
    }
    sym->name = symbol_name;
  }

  if (is_command(op))
  {
    if (sym != NULL)
    {
      sym->type = code;
      sym->address = *ic;
    }

    binary = handle_command(arg, line, i, ic, file_name, line_num);

    if (*commands == NULL)
      *commands = binary;
    else
      add_node_to_end(*commands, binary);
  }
  else if (is_directive(op))
  {
    if (sym != NULL)
    {
      sym->type = data_sym;
      sym->address = *dc;
    }

    binary = handle_directive(arg, line, i, dc, file_name, line_num, externs, entries);

    if (*data_bin == NULL)
      *data_bin = binary;
    else
      add_node_to_end(*data_bin, binary);
  }
  else
  {
    printf("Undefined operation in file %s line %d.\n", file_name, line_num);
    if (sym != NULL)
      free_symbol(sym);
    free(arg);
    return;
  }
  
  /*Meaning handling the command or directive failed.*/
  if (binary == NULL)
  {
    if (sym != NULL)
      free_symbol(sym);
    return;
  }

  if (sym != NULL)
  {
    sym_node = create_node(sym);
    if (*symbols_list == NULL)
      *symbols_list = sym_node;
    else
      add_node_to_end(*symbols_list, sym_node);
  }
}

void first_stage(char* base_file, Node* macro_list, Node** symbols_list, Node** commands, Node** data_bin, int* icf, int* dcf, Node** externs, Node** entries)
{
  char* first_arg = NULL;
  char* line = NULL;
  int i;
  int line_num = 0;
  FILE* input_file = NULL;
  char* input_file_name = create_full_file_name(base_file, FIRST_STAGE_INPUT_SUFFIX);
  operation op;
  Node* binary = NULL;
  bool error_reached = false; /*Needed for get_file_line, will be ignored*/

  /*Check for memory allocation error when creating the file name*/
  if (input_file_name == NULL)
    return;

  input_file = fopen(input_file_name, "r");
  
  /*Check that file was opened properly*/
  if (input_file == NULL)
  {
    free(input_file_name);
    return;
  }

  while ((line = get_file_line(input_file, input_file_name, &error_reached)))
  {
    i = 0;
    line_num++;
    /*Check if the line is a comment*/
    if (is_line_comment(line) || is_line_whitespaces(line, &i))
      continue;
    
    /*Handling line containing a symbol definition*/
    if (has_symbol(line, input_file_name, line_num))
    {
      handle_symbol(line, &i, symbols_list, macro_list, icf, dcf, input_file_name, line_num, commands, data_bin, externs, entries);
      continue;
    }

    first_arg = get_argument(line, &i, line_num, input_file_name, false, false);

    /*Handling parsing failure*/
    if (first_arg == NULL)
      continue;

    op = get_operation(first_arg);

    if (is_command(op))
    {
      binary = handle_command(first_arg, line, &i, icf, input_file_name, line_num);

      if (*commands == NULL)
        *commands = binary;
      else
        add_node_to_end(*commands, binary);

    }
    /*Handle a directive*/
    else if (is_directive(op))
    {
      binary = handle_directive(first_arg, line, &i, dcf, input_file_name, line_num, externs, entries);

      if (*data_bin == NULL)
        *data_bin = binary;
      else
        add_node_to_end(*data_bin, binary);
    }
    else
      printf("Undefined operation in file %s line %d.\n", input_file_name, line_num);
  }
  free(input_file_name);
}

#ifdef DEBUG_FIRST_STAGE
  int main()
  {
    int i = 4;
    int j;
    int dc = 0;
    Node* head = NULL;
    Node* temp = NULL;
    Node* entries = NULL;
    Node* externs = NULL;
    binary_or_str* tmp = NULL;
    char* operation = (char*) malloc(10);
    strcpy(operation, "add\0");

    printf("Should be 1 : %d. Should be 0 : %d.\n", has_symbol("Symbol: hello world", "my", 0), has_symbol("Nope no symbols here", "nope", 1));
    
    head = handle_command(operation, "add #3, MAT[r2][r3]", &i, &dc, "hello world", 3);
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
    printf("\n");
    teardown_linked_list(head, (void (*)(void *)) free_binary_or_str);
    
    operation = (char*) malloc(10);
    strcpy(operation, ".mat");

    i = 4;
    dc = 0;
    head = handle_directive(operation, ".mat [2][2] 1, 2, 3, 4", &i, &dc, "hello world", 4, &externs, &entries);
    
    operation = (char*) malloc(10);
    strcpy(operation, ".string");
    
    i = 7;
    temp = handle_directive(operation, ".string \"abcdef\"", &i, &dc, "hello world", 5, &externs, &entries);
    
    add_node_to_end(head, temp);
    temp = head;
    while (temp != NULL)
    {
      for (j = 0; j < BITS_IN_BYTE; j++)
        printf("%d ", ((binary*)(temp->data))->bits[j]);
      printf("\n");
      temp = temp->next;
    }
    
    teardown_linked_list(head, (void (*)(void*)) free_binary);
    free(operation);
    return 0;
  }
#endif
