#include "pre_assembler.h"
#include "boolean.h"
#include "linked_list.h"
#include "macro.h"

int main(int argc, char** argv)
{
  bool error_reached;
  int i;
  Node* macros = NULL;
  
  for (i = 1; i < argc; i++)
  {
    error_reached = false;
    macros = pre_assembler(argv[i], &error_reached);
    teardown_linked_list(macros, (void (*)(void *)) free_macro);
  }
  return 0;
}
