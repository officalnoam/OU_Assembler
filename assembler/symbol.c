#include "symbol.h"
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>

symbol* create_symbol(char* name, symbol_type type, int address)
{
  symbol* sym = (symbol*) malloc(sizeof(symbol));
  
  if (sym == NULL)
  {
    perror("Memory allocation failed.\n");
    return NULL;
  }
  
  sym->name = name;
  sym->type = type;
  sym->address = address;
  return sym;
}

symbol* find_symbol_in_list(Node* head, char* name)
{
  symbol* sym = NULL;
  
  while (head != NULL)
  {
    sym = (symbol*) head->data;
    if (strcmp(sym->name, name) == 0)
      return sym;
    head = head->next;
  }

  return NULL;
}

void free_symbol(symbol* sym)
{
  free(sym->name);
  free(sym);
}

#ifdef DEBUG_SYMBOL
  int main()
  {
    symbol* sym = NULL;
    symbol* sym2 = NULL;
    symbol* tmp = NULL;
    Node* head = NULL;
    char* name_1 = NULL;
    char* name_2 = NULL;

    name_1 = (char*) malloc(20);
    strcpy(name_1, "Hello world");
    name_2 = (char*) malloc(20);
    strcpy(name_2, "Hello world 2");

    sym = create_symbol(name_1, data, 10);
    head = create_node(sym);
    sym2 = create_symbol(name_2, data, 11);
    head->next = create_node(sym2);
    tmp = (symbol*) head->data;
    printf("%s\n", tmp->name);
    tmp = (symbol*) head->next->data;
    printf("%s\n", tmp->name);
    tmp = NULL;
    tmp = find_symbol_in_list(head, "Hello world 2");
    printf("Should be 1: %d\n", tmp == sym2);
    teardown_linked_list(head, (void (*)(void *)) free_symbol);
    return 0;
  }
#endif
