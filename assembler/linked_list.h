#ifndef LINKED_LIST
  #define LINKED_LIST
  
  typedef struct Node {
    void* data;
    struct Node* next;
  } Node;
  
  /*
  This function will create a node from the passed data.
  
  Input-
    void* data: The data to set the node's data to be.
  Output-
    Node* a pointer to the created node.
  */
  Node* create_node(void* data);

  /*
  This function will fully teardown a linked list- until the final node is reached (signified by its next being NULL).
  
  Input-
    Node* head: The head of the list.
    void* freeDataFunction(void*): If not NULL, the function to use to free the data.
  */
  void teardown_linked_list(Node* head, void (*free_data_function)(void*));

  /*
  This function will add a node to the end of a linked list.
  
  Input-
    Node* head: The head of the list.
    Node* new_node: The node being added.
  */
  void add_node_to_end(Node* head, node* new_node);
#endif
