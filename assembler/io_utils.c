#include "io_utils.h"
#include <stdlib.h>

#ifdef DEBUG_IO
  #include <string.h>
#endif

/*
Will get a file, and check its position.
Will then go through the file until the position is reached again, counting the '\n' until the position reached.
*/
int get_current_line_num(FILE* file)
{
  int line_number = 1;
  long current_position = ftell(file);
  char c;
  
  /*Set the position in the file to the start of the file.*/
  fseek(file, 0, SEEK_SET);
  
  /*Go over the chars until the current position and count the newlines*/
  while((c = fgetc(file)) != EOF && ftell(file) != current_position)
  {
    if (c == '\n')
      line_number++;
  }
  
  return line_number;
}

/*
Get a line from the passed file.
*/
char* get_file_line(FILE* file, char* file_name, bool* error_reached)
{
  char* file_line = (char*) malloc(LINE_LEN * sizeof(char));
  char c;
  int i = 0;
  
  /*Check that memory was allocated properly.*/
  if (file_line == NULL)
  {
    perror("Memory allocation failed.\n");
    *error_reached = true;
    return NULL;
  }
  
  while ((c = fgetc(file)) != '\n' && c != EOF)
  {
    if (i == LINE_LEN - 2)
    {
      printf("ERROR: Error in line %s in line %d - line is over %d characters long.\n", file_name, get_current_line_num(file), LINE_LEN);
      free(file_line);
      *error_reached = true;
      return NULL;
    }
    file_line[i] = c;
    i++;
  }

  if (c != EOF)
  {
    /*Add a newline and string ending char to the parsed line*/
    file_line[i] = '\n';
    file_line[i + 1] = '\0';
  }
  else
  {
    /*If EOF is the only char parsed- return NULL to signify that EOF reached*/
    if (i == 0)
    {
      free(file_line);
      return NULL;
    }
  }
 
  return file_line;
}

/*
Write a line into a file.
*/
void write_file_line(FILE* file, char* file_line)
{
  fputs(file_line, file);
}


/*
A debug main function to specifically test the functionalities in the file- getting a base file name, and then reading the input file
and then copying each line twice to a newly created file.
*/
#ifdef DEBUG_IO
  int main(int argc, char** argv)
  {
    char input_file_name[20];
    char output_file_name[20];
    FILE* input_file = NULL;
    FILE* output_file = NULL;
    char* file_line;
    bool error_reached = false;

    strncpy(input_file_name, argv[1], 20);
    strncpy(output_file_name, argv[1], 20);
    strncat(input_file_name, ".as", 3);
    strncat(output_file_name, ".am", 3);
    
    input_file = fopen(input_file_name, "r");
    
    if (input_file == NULL)
    {
      printf("Opening %s failed.\n", input_file_name);
      return 1;
    }
    
    output_file = fopen(output_file_name, "w");
    
    if (output_file == NULL)
    {
      printf("Opening %s failed.\n", output_file_name);
      fclose(input_file);
      return 1;
    }
    
    /*Write every line in the input file twice into the output file*/    
    while ((file_line = get_file_line(input_file, input_file_name, &error_reached)) != NULL)
    {
      write_file_line(output_file, file_line);
      write_file_line(output_file, file_line);
    }

    printf("Error reached? %d\n", error_reached);
    fclose(input_file);
    fclose(output_file);
    return 0;
  }
#endif
