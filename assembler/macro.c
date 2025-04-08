#include "macro.h"
#ifdef DEBUG_MACRO
  #include "linked_list.h"
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
Create a macro, set all the pointers to strings within the lines array of string, to point to NULL.
*/
macro* create_macro(char* macro_name)
{
  int i;
  macro* mac = (macro*) malloc(sizeof(macro));
  
  /*Check memory allocation success*/
  if (mac == NULL)
  {
    perror("Memory allocation failed.\n");
    return NULL;
  }

  /*Initialize all of the macro's fields*/
  mac->name = macro_name;
  mac->lines = (char**) malloc(sizeof(char*) * LINE_BUFFER);
  mac->line_amount = 0;
  mac->line_capacity = LINE_BUFFER;
  
  /*Check macro line initialization success.*/
  if (mac->lines == NULL)
  {
    perror("Memory allocation failed.\n");
    free(mac);
    return NULL;
  }
  
  /*Set all of the pointers in the line array to NULL.*/
  for (i = 0; i < LINE_BUFFER; i++)
    mac->lines[i] = NULL;

  return mac; 
}

/*
Check that the lines in the macro has space to add another line- if not make sure to reallocate space for the lines field.
Add the line to the next line within the lines array.
*/
void add_line_to_macro(macro* mac, char* line)
{
  int i;
  char** temp = NULL;

  if (mac->line_amount + 1 == mac->line_capacity)
  {
    mac->line_capacity = mac->line_capacity + LINE_BUFFER;
    temp = (char**) realloc(mac->lines, sizeof(char*) * mac->line_capacity);
    
    /*Validate the memory reallocation*/
    if (temp == NULL)
    {
      perror("Memory reallocation failed");
      free_macro(mac);
      mac = NULL;
      return;
    }
    else if (mac->lines != temp)
    {
      mac->lines = temp;
    } 
    
    /*Initialize all of the new pointers in the line array to NULL*/
    for (i = mac->line_amount; i < mac->line_capacity; i++)
      mac->lines[i] = NULL;
  }
  
  mac->lines[mac->line_amount] = line;
  mac->line_amount++;
}

/*
Go over the entire linked list- compare the items to the macro name, return if it's matching.
*/
macro* find_macro_in_list(Node* head, char* macro_name)
{
  macro* mac = NULL;

  while (head != NULL)
  {
    mac = (macro*) head->data;
    if (strcmp(mac->name, macro_name) == 0)
      return mac;
    head = head->next;
  }
  return NULL;
}

/*
Free the macro name, all lines within the lines array, the array itself, and the macro itself.
*/
void free_macro(macro* mac)
{
  int i;

  free(mac->name);
  for (i = 0; i < mac->line_amount; i++)
    free(mac->lines[i]);
  free(mac->lines);
  free(mac);
}

/*
Debug main- to check all of the functions within the file.
Manually allocate memory for all strings- to properly check free_macro.
*/
#ifdef DEBUG_MACRO
  int main()
  {
    macro* mac = NULL;
    macro* mac2 = NULL;
    macro* tmp = NULL;
    Node* head = NULL;
    char* name_1 = NULL;
    char* name_2 = NULL;
    char* line_1 = NULL;
    char* line_2 = NULL;
    char* line_3 = NULL;
    int i;
   
    name_1 = (char*) malloc(20);
    strcpy(name_1, "Hello world");
    name_2 = (char*) malloc(20);
    strcpy(name_2, "Hello world 2");
    line_1 = (char*) malloc(20);
    strcpy(line_1, "Line 1\n");
    line_2 = (char*) malloc(20);
    strcpy(line_2, "Line 2\n");
    line_3 = (char*) malloc(20);
    strcpy(line_3, "Line 3\n");

    mac = create_macro(name_1);
    add_line_to_macro(mac, line_1);
    add_line_to_macro(mac, line_2);
    add_line_to_macro(mac, line_3);
    
    for (i = 0; i < mac->line_amount; i++)
      printf("%s", mac->lines[i]);
    
    head = create_node(mac);
    mac2 = create_macro(name_2);
    head->next = create_node(mac2);
    tmp = (macro*) head->data;
    printf("%s\n", tmp->name);
    tmp = (macro*) head->next->data;
    printf("%s\n", tmp->name);
    tmp = NULL;
    teardown_linked_list(head, (void (*)(void *)) free_macro);
    return 0;
  }
#endif
