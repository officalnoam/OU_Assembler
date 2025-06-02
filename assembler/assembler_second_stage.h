#ifndef SECOND_STAGE
  #include "linked_list.h"
  #include "binary_utils.h"
  #include "boolean.h"
  #include <stdio.h>

  #define SECOND_STAGE
  #define BASE_ADDRESS 100
  #define OBJECT_FILE_SUFFIX ".ob"
  #define EXTERNALS_FILE_SUFFIX ".ext"
  #define ENTRIES_FILE_SUFFIX ".ent"
  #define SPACES  "     "
  #define FILE_LINE_LEN 20
  #define BITS_IN_ADDRESS 8

  typedef struct {
    char* name;
    int address;
  } external_address;

  /*
  This function will remove entries that do not exist in the symbols list.

  Input-
    Node* symbols_list: The symbols list.
    Node** entries: A pointer to the linked list of the entries.
  */
  void remove_nonexistant_entries(Node* symbols_list, Node** entries);

  /*
  This function will get the amount of bytes the command was converted to.

  Input-
    binary* command_byte: The first byte in a command's conversion to binary.
  Output-
    int: The amount of bytes the command was converted to.
  */
  int get_command_arg_byte_num(binary* command_byte);

  /*
  This function will check if a symbol is external.

  Input-
    Node* externs: A linked list of the externals.
    char* symbol_name: The name of the symbol.
  Output-
    bool: true if the symbol is external, false otherwise.
  */
  bool check_if_symbol_in_externals(Node* externs, char* symbol_name);
  
  /*
  This function will change the addresses of all symbols, from a certain address, by a certain amount.

  Input-
    Node* symbols_list: The linked list of the symbols.
    int min_address: The minimum address from which the addresses should be changed.
    int change: The amount to remove from the addresses.
  */
  void change_symbols_addresses(Node* symbol_list, int min_address, int change);

  /*
  This function will remove invalid commands from the command list-
  specifically, this will go over each binary_or_str and check if the symbols references try and use
  a symbol that doesn't exist (either as a normal symbol or an external).
  If such a bos exists- the entire command is removed from the linked list.

  Input-
    Node** commands: The pointer to the commands linked list.
    Node* symbols_list: The linked list of the symbols.
    Node* externs: The linked list of the externals.
    int* icf: A pointer to the icf.
  */
  void remove_invalid_commands(Node** commands, Node* symbols_list, Node* externs, int* icf);

  /*
  This function will finalize the address of all symbols.
  This will be done by adding the base address to all addresses, and the icf to symbols referencing data binary.

  Input-
    Node* symbols_list: The linked list of the symbols.
    int icf: The icf.
  */
  void finalize_symbols(Node* symbols_list, int icf);

  /*
  This function will go over all binary_or_str in the commands linked list, add the binary ones
  to a new binary linked list, and convert the strs to binary.
  
  Input-
    Node* commands: The linked list of the commands.
    Node* symbols_list: The linked list of the symbols.
    Node* externs: The linked list of the externs.
    Node** external_addresses: A pointer to the linked list of external addresses- which will store the addresses of each use of an external.
  Output-
    Node*: The created linked list, of the commands as a binary linked list.
  */
  Node* convert_commands_to_binary(Node* commands, Node* symbols_list, Node* externs, Node** external_addresses);
  
  /*
  This function will convert two binarys into base 4, and write them as a line in the file.

  Input-
    FILE* file: The file being written to.
    binary* bin1: The first binary.
    binary* bin2: The second binary.
  */
  void write_line_to_file_two_bins(FILE* file, binary* bin1, binary* bin2);
  
  /*
  This function will convert one binary into base 4, and the write it and a string as a line in the file.

  Input-
    FILE* file: the file being written to.
    binary* bin: The binary.
    char* str: The string.
  */
  void write_line_to_file_one_bin_one_str(FILE* file, binary* bin, char* str);
  
  /*
  This function will teardown all of the passed linked lists, if they are not equal to NULL.

  Input-
    Node* commands_bin: The linked list of the commands as binary.
    Node* external_addresses: The linked list of the external addresses.
    Node* symbols_list: The linked list of the symbols.
    Node* commands: The linked list of the commands as binary_or_str.
    Node* data_bin: The linked list of the data.
    Node* externs: The linked list of the externals.
    Node* entries: The linked list of the entries.
  */
  void teardown_all_linked_lists(Node* commands_bin, Node* external_addresses, Node* symbols_list, Node* commands, Node* data_bin, Node* externs, Node* entries);
  
  /*
  This function will convert the commands into proper binary, write it and the data into an .ob file, 
  and write .ext and .ent files if relevant.

  Input-
    char* base_file: The base file name before suffixes.
    Node* symbols_list: The linked list of the symbols.
    Node* commands: The linked list of the commands as binary_or_str.
    Node* data_bin: The linked list of the data.
    Node* externs: The linked list of the externs.
    Node* entries: The linked list of the entries.
    int icf: The instructions counter.
    int dcf: The data counter.
  */
  void second_stage(char* base_file, Node* symbols_list, Node* commands, Node* data_bin, Node* externs, Node* entries, int icf, int dcf);
#endif