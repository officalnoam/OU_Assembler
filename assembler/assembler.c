#include "pre_assembler.h"
#include "boolean.h"

int main(int argc, char** argv)
{
  bool error_reached;
  int i;
  
  for (i = 1; i < argc; i++)
  {
    error_reached = false;
    pre_assembler(argv[i], &error_reached);
  }
  return 0;
}
