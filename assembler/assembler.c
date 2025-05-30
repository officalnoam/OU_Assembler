#include "pre_assembler.h"
#include "boolean.h"
#include "linked_list.h"
#include "macro.h"
#include "assembler_first_stage.h"

int main(int argc, char** argv)
{
  bool error_reached;
  int i;
  Node* macros = NULL;
  Node* symbols_list = NULL;
  Node* commands = NULL;
  Node* data_bin = NULL;
  Node* externs = NULL;
  Node* entries = NULL;
  int icf = 0;
  int dcf = 0;
  
  for (i = 1; i < argc; i++)
  {
    error_reached = false;
    macros = pre_assembler(argv[i], &error_reached);
    first_stage(argv[i], macros, &symbols_list, &commands, &data_bin, &icf, &dcf, &externs, &entries);
    teardown_linked_list(macros, (void (*)(void *)) free_macro);
  }
  return 0;
}
