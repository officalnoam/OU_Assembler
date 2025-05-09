#ifndef BINARY_UTILS
  #define BINARY_UTILS
  #define BITS_IN_BYTE 10
  #define A_IN_ASCII 97
  #define BYTE_IN_BASE_FOUR 5
  
  typedef struct {
    int* bits;
    int size;
  } binary;
  
  /*
  This function will convert a binary to the negative representation of the same number.
  
  Input-
    binary* bin: The binary being converted.
  */
  void convert_bin_to_negative(binary* bin);

  /*
  This function will convert a char representing the num- which starts with a # and whose second character can be a -, + or the start of the number
  to binary.
  
  Input-
    int num: The number being converted.
    int bit_amount: The bit amount for the binary object. 
  Output-
    binary*: The created binary.
  */  
  binary* convert_num_to_binary(int num, int bit_amount);
  
  /*
  This function will convert binary to the base four.
  
  Input-
    binary* bin: The binary being converted.
  Output-
    char*: The base 4 of the binary.
  */
  char* convert_binary_to_base_four(binary* bin);
  
  /*
  This function will create a byte- from up to 4 different passed binaries. They will be added in order.
  Any unused binary- should be passed as NULL.
  
  Input-
    binary* bin1: The first binary.
    binary* bin2: The second binary.
    binary* bin3: The third binary.
    binary* bin4: The fourth binary.
  Output-
    binary*: The created binary.
  */
  binary* create_byte(binary* bin1, binary* bin2, binary* bin3, binary* bin4);
  
  /*
  This function will completly free a binary object.
  
  Input-
    binary* bin: the binary being freed.
  */
  void free_binary(binary* bin);

  /*
  Free multiple binaries.

  Input-
    binary* bin1: The first binary.
    binary* bin2: The second binary.
    binary* bin3: The third binary.
    binary* bin4: The fourth binary.
  */
  void free_multiple_binary(binary* bin1, binary* bin2, binary* bin3, binary* bin4);
#endif
