#ifndef SYMBOL
  #include "linked_list.h"
  
  #define SYMBOL
  
  typedef enum {
    code,
    data_sym,
    external
  } symbol_type;
  
  typedef struct {
    char* name;
    symbol_type type;
    int address;
  } symbol;

  /*
  This function will create a symbol.
  
  Input-
    char* name: The name of the symbol.
    symbol_type type: The type of the symbol.
    int address: The memory address of the symbol.
  Output-
    symbol*: The created symbol.
  */  
  symbol* create_symbol(char* name, symbol_type type, int address);
  
  /*
  This function finds a symbol in a linked list, based on the symbol name.
  
  Input-
    Node* head: The head of the linked lists of symbols.
    char* name: The name of the desired linked list.
  Output-
    symbol*: The desired symbol.
  */
  symbol* find_symbol_in_list(Node* head, char* name);
  
  /*
  This function completely frees the symbol.
   
  Input-
    symbol* sym: The symbol being freed.
  */
  void free_symbol(symbol* sym);
#endif
