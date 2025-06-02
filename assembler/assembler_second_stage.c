#include "assembler_second_stage.h"
#include "symbol.h"
#include "operations.h"
#include "addressing.h"
#include "assembler_first_stage.h"
#include "file_utils.h"
#include "io_utils.h"
#include <stdlib.h>
#include <string.h>

#include <signal.h>

/*
We will create two temporary linked lists- one for valid entries and one for invalid ones.
If an entry exists within the symbols list it will enter the valid entries, otherwise the invalids.
The entries linked list is then defined to the valid entries linked list, while the invalid one is torndown.
*/
void remove_nonexistant_entries(Node* symbols_list, Node** entries)
{
  Node* non_existant_entries = NULL;
  Node* new_entries_list = NULL;
  Node* current_head = *entries;
  Node* temp_node = NULL;  

  while (current_head != NULL)
  {
    /*Save the next so that the current node can be added to a linked list without adding all nodes after it.*/
    temp_node = current_head->next;
    current_head->next = NULL;

    if (find_symbol_in_list(symbols_list, current_head->data) == NULL)
    {
      if (non_existant_entries == NULL)
        non_existant_entries = current_head;
      else
        add_node_to_end(non_existant_entries, current_head);
    }
    else
    {
      if (new_entries_list == NULL)
        new_entries_list = current_head;
      else
        add_node_to_end(new_entries_list, current_head);
    }
    current_head = temp_node;
  }
  *entries = new_entries_list;

  if (non_existant_entries != NULL)
    teardown_linked_list(non_existant_entries, free);
}

int get_command_arg_byte_num(binary* command_byte)
{
  operation op;
  addressing source;
  addressing target;
  int command_arg_amount;
  
  /*Find the operation, source and target addressing types by parsing the binary.*/
  op = command_byte->bits[0] * 8 + command_byte->bits[1] * 4 + command_byte->bits[2] * 2 + command_byte->bits[3] + 2;
  source = command_byte->bits[4] * 2 + command_byte->bits[5];
  target = command_byte->bits[6] * 2 + command_byte->bits[7];
  command_arg_amount = get_command_arg_amount(op);  

  /*If the command is either a 0 or 1 argument command, set source and/or target to undefined_addressing so it is considered 0 bits.*/
  if (command_arg_amount <= 1)
    source = undefined_addressing;
  
  if (command_arg_amount == 0)
    target = undefined_addressing;
  
  /*Handle both arguments being registers*/
  if (source == register_addressing && target == register_addressing)
    return 1;
  
  return get_addressing_bytes(target) + get_addressing_bytes(source);
}

bool check_if_symbol_in_externals(Node* externs, char* symbol_name)
{
  /*Compare every char in the externs linked list to the symbol name*/
  while (externs != NULL)
  {
    if (strcmp(externs->data, symbol_name) == 0)
      return true;
    externs = externs->next;
  }
  return false;
}

void change_symbols_addresses(Node* symbol_list, int min_address, int change)
{
    symbol* sym = NULL;

    while (symbol_list != NULL)
    {
        sym = (symbol*) symbol_list->data;

        /*If the symbol is a code symbol, and the address is after the removed commands- remove appropriate change from addressing.*/
        if (sym->type == code && sym->address > min_address)
          sym->address = sym->address - change;

        symbol_list = symbol_list->next;
    }
}

void remove_invalid_commands(Node** commands, Node* symbols_list, Node* externs, int* icf)
{
  Node* valid_commands = NULL;
  Node* invalid_commands = NULL;
  Node* current_command_head = *commands;
  Node* current_node = *commands;
  Node* temp_node = NULL;
  binary_or_str* bos = NULL;
  bool error_reached = false;
  int current_command_address = 0;
  int command_arg_bytes;
  int i;

  /*The while loop will go between commands.*/
  while (current_command_head != NULL)
  {
    bos = (binary_or_str*) current_node->data;
    command_arg_bytes = get_command_arg_byte_num(bos->bin); 
    
    /*Go over all of the argument bytes within the command and check if any of them contain a non existant symbol*/
    for (i = 0; i < command_arg_bytes; i++)
    {
      current_node = current_node->next;
      bos = (binary_or_str*) current_node->data;
      if ((bos->str != NULL) && !check_if_symbol_in_externals(externs, bos->str) && (find_symbol_in_list(symbols_list, bos->str) == NULL))
        error_reached = true;
    }

    /*Temporarily save the next node, and remove it from the last command byte so that only the command bytes are added to a linked list.*/
    temp_node = current_node->next;
    current_node->next = NULL;

    if (error_reached)
    {
      if (invalid_commands == NULL)
        invalid_commands = current_command_head;
      else
        add_node_to_end(invalid_commands, current_command_head);
      
      /*Handle changes in addressing.*/
      change_symbols_addresses(symbols_list, current_command_address, command_arg_bytes);
      *icf = *icf - command_arg_bytes;
    }
    else
    {
      current_command_address = current_command_address + 1 + command_arg_bytes;
      if (valid_commands == NULL)
        valid_commands = current_command_head;
      else
        add_node_to_end(valid_commands, current_command_head);
    }

    current_command_head = temp_node;
    current_node = temp_node;
    error_reached = false;
  }

  *commands = valid_commands;

  if (invalid_commands != NULL)
    teardown_linked_list(invalid_commands, (void (*)(void *)) free_binary_or_str);
}

void finalize_symbols(Node* symbols_list, int icf)
{
  symbol* sym = NULL;

  /*Go over every symbol, add the base address and if they are a data symbol add the icf too.*/
  while (symbols_list != NULL)
  {
    sym = (symbol*) symbols_list->data;

    sym->address = sym->address + BASE_ADDRESS;
    
    if (sym->type == data_sym)
      sym->address = sym->address + icf;

    symbols_list = symbols_list->next;
  }
}

Node* convert_commands_to_binary(Node* commands, Node* symbols_list, Node* externs, Node** external_addresses)
{
  Node* commands_binary = NULL;
  Node* temp_node = NULL;
  binary_or_str* bos = NULL;
  binary* bin1 = NULL;
  binary* bin2 = NULL;
  binary* byte = NULL;
  symbol* sym = NULL;
  external_address* ea = NULL;
  int current_address = BASE_ADDRESS;

  while (commands != NULL)
  {
    bos = (binary_or_str*) commands->data;

    /*If the byte is already binary, copy the created binary and then change the binary in commands 
    to NULL to avoid double freeing.*/
    if (bos->bin != NULL)
    {
      byte = bos->bin;
      bos->bin = NULL;
    }
    else
    {
      /*If the symbol is external- set the value to 0. Adding the command address to the external addresses so we can create the externals file properly.*/
      if (check_if_symbol_in_externals(externs, bos->str))
      {
        bin1 = convert_num_to_binary(0, BITS_EXCLUDING_ARE, true);
        bin2 = convert_num_to_binary(1, BITS_IN_ARE, true);

        ea = (external_address*) malloc(sizeof(external_address));
        ea->name = bos->str;
        ea->address = current_address;
        temp_node = create_node(ea);

        if (*external_addresses == NULL)
          *external_addresses = temp_node;
        else
          add_node_to_end(*external_addresses, temp_node);
      }

      /*If the symbol is a proper symbol - convert its address to binary and set that to the value*/
      else
      {        
        sym = find_symbol_in_list(symbols_list, bos->str);
        bin1 = convert_num_to_binary(sym->address, BITS_EXCLUDING_ARE, true);
        bin2 = convert_num_to_binary(2, BITS_IN_ARE, true);
      }

      byte = create_byte(bin1, bin2, NULL, NULL);
      free_multiple_binary(bin1, bin2, NULL, NULL);
    }

    temp_node = create_node(byte);

    if (commands_binary == NULL)
      commands_binary = temp_node;
    else
      add_node_to_end(commands_binary, temp_node);

    current_address++;
    commands = commands->next;
  }
  return commands_binary;
}

void write_line_to_file_two_bins(FILE* file, binary* bin1, binary* bin2)
{
  char file_line[FILE_LINE_LEN];
  char* str1 = NULL;
  char* str2 = NULL;

  str1 = convert_binary_to_base_four(bin1);
  str2 = convert_binary_to_base_four(bin2);
  
  strcpy(file_line, str1);
  strcat(file_line, SPACES);
  strcat(file_line, str2);
  strcat(file_line, "\n");
  
  write_file_line(file, file_line);

  free(str1);
  free(str2);
}

void write_line_to_file_one_bin_one_str(FILE* file, binary* bin, char* str)
{
  char file_line[FILE_LINE_LEN];
  char* str2 = NULL;

  str2 = convert_binary_to_base_four(bin);

  strcpy(file_line, str);
  strcat(file_line, SPACES);
  strcat(file_line, str2);
  strcat(file_line, "\n");

  write_file_line(file, file_line);

  free(str2);
}

void teardown_all_linked_lists(Node* commands_bin, Node* external_addresses, Node* symbols_list, Node* commands, Node* data_bin, Node* externs, Node* entries)
{
  if (commands_bin != NULL)
    teardown_linked_list(commands_bin, (void (*)(void *)) free_binary);
    
  if (external_addresses != NULL)
    teardown_linked_list(external_addresses, NULL);

  if (symbols_list != NULL)
    teardown_linked_list(symbols_list, (void (*)(void *)) free_symbol);
  
  if (commands != NULL)
    teardown_linked_list(commands, (void (*)(void *)) free_binary_or_str);
  
  if (data_bin != NULL)
    teardown_linked_list(data_bin, (void (*)(void *)) free_binary);
  
  if (externs != NULL)
    teardown_linked_list(externs, free);
  
  if (entries != NULL)
    teardown_linked_list(entries, free);
}

void second_stage(char* base_file, Node* symbols_list, Node* commands, Node* data_bin, Node* externs, Node* entries, int icf, int dcf)
{
  char* file_name = NULL;
  FILE* output_file = NULL;
  Node* commands_bin = NULL;
  Node* external_addresses = NULL;
  binary* temp_bin = NULL;
  binary* temp_bin2 = NULL;
  int current_address = BASE_ADDRESS;
  Node* temp_node = NULL;
  symbol* sym = NULL;
  external_address* ea = NULL;

  /*Open the object file*/
  file_name = create_full_file_name(base_file, OBJECT_FILE_SUFFIX);
  if (file_name == NULL)
  {
    teardown_all_linked_lists(commands_bin, external_addresses, symbols_list, commands, data_bin, externs, entries);
    return;
  }
  output_file = fopen(file_name, "w");
  
  if (output_file == NULL)
  {
    teardown_all_linked_lists(commands_bin, external_addresses, symbols_list, commands, data_bin, externs, entries);
    return;
  }
  
  /*Prepare linked lists for command binary conversion*/
  remove_nonexistant_entries(symbols_list, &entries);
  remove_invalid_commands(&commands, symbols_list, externs, &icf);
  finalize_symbols(symbols_list, icf);
  
  /*Convert the commands into bin*/
  commands_bin = convert_commands_to_binary(commands, symbols_list, externs, &external_addresses);
  

  /*Write the object file*/


  /*Write the icf dcf line*/
  temp_bin = convert_num_to_binary(icf, BITS_IN_ADDRESS, true);  
  temp_bin2 = convert_num_to_binary(dcf, BITS_IN_ADDRESS, true);
  
  write_line_to_file_two_bins(output_file, temp_bin, temp_bin2);
  write_file_line(output_file, "\n\n");
  
  free(temp_bin);
  free(temp_bin2);

  /*Adding the data_bin to the end of the commands_bin will create one unified linked list which will be converted
  to base 4 and the written to the file. From this point, when tearing down lists, we won't pass data_bin
  since it will be torn down as part of commands_bin*/
  add_node_to_end(commands_bin, data_bin);
  
  /*Write all of the binary lines*/
  temp_node = commands_bin;
  while (temp_node != NULL)
  {
    temp_bin = convert_num_to_binary(current_address, BITS_IN_ADDRESS, true);
    temp_bin2 = (binary*) temp_node->data;

    write_line_to_file_two_bins(output_file, temp_bin, temp_bin2);
    
    free(temp_bin);
    current_address++;
    temp_node = temp_node->next;
  }

  free(file_name);
  fclose(output_file);

  /*If entries exists- handle writing the entries file*/

  if (entries != NULL)
  {
    /*Open the entries file*/
    file_name = create_full_file_name(base_file, ENTRIES_FILE_SUFFIX);
    if (file_name == NULL)
    {
      teardown_all_linked_lists(commands_bin, external_addresses, symbols_list, commands, NULL, externs, entries);
      return;
    }
    
    output_file = fopen(file_name, "w");
    
    if (output_file == NULL)
    {
      teardown_all_linked_lists(commands_bin, external_addresses, symbols_list, commands, NULL, externs, entries);
      return;
    }

    /*Write every entry as a line within the file*/
    temp_node = entries;
    while (temp_node != NULL)
    {
      /*sym != NULL due to us preparing the symbols list.*/
      sym = find_symbol_in_list(symbols_list, temp_node->data);
      temp_bin = convert_num_to_binary(sym->address, BITS_IN_ADDRESS, true);

      write_line_to_file_one_bin_one_str(output_file, temp_bin, sym->name);

      free(temp_bin);
      temp_node = temp_node->next;
    }

    free(file_name);
    fclose(output_file);
  }

  /*If externals addresses exist- handle writing the externals file*/
  if (external_addresses != NULL)
  {
    /*Open the externals file*/
    file_name = create_full_file_name(base_file, EXTERNALS_FILE_SUFFIX);
    if (file_name == NULL)
    {
      teardown_all_linked_lists(commands_bin, external_addresses, symbols_list, commands, NULL, externs, entries);
      return;
    }
    
    output_file = fopen(file_name, "w");

    if (output_file == NULL)
    {
      teardown_all_linked_lists(commands_bin, external_addresses, symbols_list, commands, NULL, externs, entries);
      return;
    }
    
    /*Go over every external address, and write it as a line within the file*/
    temp_node = external_addresses;
    while (temp_node != NULL)
    {
      ea = (external_address*) temp_node->data;
      temp_bin = convert_num_to_binary(ea->address, BITS_IN_ADDRESS, true);

      write_line_to_file_one_bin_one_str(output_file, temp_bin, ea->name);

      free(temp_bin);
      temp_node = temp_node->next;
    }

    free(file_name);
    fclose(output_file);
  }

  teardown_all_linked_lists(commands_bin, external_addresses, symbols_list, commands, NULL, externs, entries);  
}