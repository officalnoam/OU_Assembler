#include "file_utils.h"
#include <string.h>

/*
Manually allocate memory to a new variable- which is base_file_name + suffix.
*/
char* create_full_file_name(char* base_file_name, char* suffix)
{
  char* file_name;
  int size = strlen(base_file_name) + strlen(suffix) + 1;

  file_name = (char*) malloc(size * sizeof(char));
  if (file_name == NULL)
  {
    perror("Memory allocation failed.\n");
    return NULL;
  }
  
  strcpy(file_name, base_file_name);
  strncat(file_name, suffix, strlen(suffix));
  return file_name;
} 

/*
Open input file as a read file, output file as a write file.
*/
void open_files(FILE** input_file, FILE** output_file, char* input_file_name, char* output_file_name)
{
  *input_file = fopen(input_file_name, "r");
  *output_file = fopen(output_file_name, "w");
}