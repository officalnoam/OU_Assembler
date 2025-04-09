#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>

/*
This function will create a Node with the passed data, and a next set to NULL).
*/
Node* create_node(void* data)
{
  Node* node = (Node*) malloc(sizeof(Node));
  
  if (node == NULL)
  {
    perror("Memory allocation failed");
    return NULL;
  }
  
  node->data = data;
  node->next = NULL;
  return node;
}

/*
Teardown all nodes in the list by freeing them, and if needed their data.
*/
void teardown_linked_list(Node* head, void (*free_data_function)(void*))
{
  Node* prev = NULL;
  while (head != NULL)
  {
    if (free_data_function != NULL)
      free_data_function(head->data);
    
    prev = head;
    head = head->next;
    free(prev);
  }
}

/*Go over the entire linked list until the last item is reached, and then set its next to the new node*/
void add_node_to_end(Node* head, Node* new_node)
{
  while (head->next != NULL)
    head = head->next;
  head->next = new_node;
}

/*
A debug main, specifically checking the functions within the file- creating nodes and freeing linked lists.
*/
#ifdef DEBUG_LINKED_LIST
  int main()
  {
    int arr[] = {1, 2, 3, 4, 5, 6};
    int i;

    Node* head = create_node(&arr[0]);
    Node* tmp = head;
    for (i = 1; i < 6; i++)
    {
      tmp->next = create_node(&arr[i]);
      tmp = tmp->next;
    } 
    
    tmp = head;
    for (i = 0; i < 6; i++)
    {
      printf("%d\n", *((int*)tmp->data));
      tmp = tmp->next;
    }
    teardown_linked_list(head, NULL);
    return 0;
  }
#endif
