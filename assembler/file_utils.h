#ifndef FILE_UTILS
  #include <stdio.h>
  #define FILE_UTILS

  /*
  This function will create a full file name from the base file name and a suffix.
  
  Input-
    char* base_file_name: The base file name.
    char* suffix: The suffix being added.
  Output-
    char*: The full file name.
  */
  char* create_full_file_name(char* base_file_name, char* suffix);

  /*
  This function will open an input and output file.
  
  Input-
    FILE** input_file: The input file.
    FILE** output_file: The output file.
    char* input_file_name: The name of the input file.
    char* output_file_name: The name of the output file.
  */
  void open_files(FILE** input_file, FILE** output_file, char* input_file_name, char* output_file_name);
#endif