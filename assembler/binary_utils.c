#include "binary_utils.h"
#include "boolean.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*Change all 0s to 1 and vice versa, and then add 1*/
void convert_bin_to_negative(binary* bin)
{
  int i;
  
  /*Flip all bits*/
  for (i = 0; i < bin->size; i++)
    bin->bits[i] = (bin->bits[i] + 1) % 2;
  
  /*Add one to the binary*/
  for (i = bin->size - 1; i > 0; i--)
  {
    if (bin->bits[i] == 0)
      bin->bits[i] = 1;
    else
    {
      bin->bits[i] = 0;
      bin->bits[i-1] = (bin->bits[i-1] + 1) % 2;
    }
  }
}
 
/*
Save the int as binary- by checking the % of 2 and dividing by 2. Save the first bit as 0 as a sign signifier.
Push all of the bits to the end of the binary object.
If the number is negative- convert the binary into a negative.
*/
binary* convert_num_to_binary(int num, int bit_amount)
{
  int i = 1;
  int temp = num;
  bool is_negative = false;
  binary* bin = (binary*) malloc(sizeof(binary));
  
  if (bin == NULL)
  {
    perror("Memory allocation error.\n");
    return NULL;
  }
  
  bin->bits = (int*) malloc(sizeof(int) * bit_amount);
  
  if (bin->bits == NULL)
  {
    perror("Memory allocation error.\n");
    return NULL;
  }

  if (num < 0)
  {
    temp = num * -1;
    is_negative = true;
  }
  
  bin->size = 1;
  bin->bits[0] = 0;
  
  /*Convert num to binary, without dealing with sign yet*/
  while (temp != 0)
  {
    if (i == bit_amount)
    {
      printf("Error: number %d is out of range for supported nums.\n", num);
      free_binary(bin);
      return NULL;
    }

    bin->bits[i] = temp % 2;
    temp = temp / 2;
    i++;
    bin->size++;
  }
  
  /*Push all of the bits to the left side of the binary*/
  for (i = 1; i < bit_amount - bin->size + 1; i++)
  {
    bin->bits[bit_amount - i] = bin->bits[i];
    bin->bits[i] = 0;
  };
  bin->size = bit_amount;

  if (is_negative)
    convert_bin_to_negative(bin);
  
  return bin;
}

/*
If the binary is of an odd size- convert the first bit to the four base- a b c d.
Then, go over the entire binary, two items at a time, and convert each pair into a b c d.
*/
char* convert_binary_to_base_four(binary* bin)
{
  int i = 0;
  int j = 0;
  char* str = (char*) malloc((BYTE_IN_BASE_FOUR + 1) * sizeof(char));

  if (str == NULL)
  {
    printf("Memory allocation failure.\n");
    return NULL;
  }
  
  if (bin->size % 2 == 1)
  {
    str[j] = (char) (bin->bits[i] + A_IN_ASCII);
    i++;
    j++; 
  }
  
  for (; i < bin->size; i += 2)
  {
    str[j] = (char) (bin->bits[i] * 2 + bin->bits[i + 1] + A_IN_ASCII);
    j++;
  }
  str[j] = '\0';
  return str;
}

/*
Create a new binary object, and add each non NULL passed binary in order into the new binary object.
*/
binary* create_byte(binary* bin1, binary* bin2, binary* bin3, binary* bin4)
{
  int i;
  int j = 0;
  binary* bin = (binary*) malloc(sizeof(binary));
  
  if (bin == NULL)
  {
    perror("Memory allocation failed.\n");
    return NULL;
  }
  
  bin->bits = (int*) malloc(BITS_IN_BYTE * sizeof(binary));
 
  for (i = 0; i < BITS_IN_BYTE; i++)
    bin->bits[i] = 0;
  bin->size = BITS_IN_BYTE;
 
  if (bin == NULL)
  {
    perror("Memory allocation failed.\n");
    free(bin);
    return NULL;
  }
  
  if (bin1 != NULL)
  {
    for (i = 0; i < bin1->size; i++)
    {
      bin->bits[j] = bin1->bits[i];
      j++;
    }
  }
  if (bin2 != NULL)
  {
    for (i = 0; i < bin2->size; i++)
    {
      bin->bits[j] = bin2->bits[i];
      j++;
    }
  }
  if (bin3 != NULL)
  {
    for (i = 0; i < bin3->size; i++)
    {
      bin->bits[j] = bin3->bits[i];
      j++;
    }
  }
  if (bin4 != NULL)
  {
    for (i = 0; i < bin4->size; j++)
    {
      bin->bits[j] = bin3->bits[i];
      j++;
    }
  }

  return bin;
}

void free_binary(binary* bin)
{
  free(bin->bits);
  free(bin);
}

/*
Debug main to specifically check the functions within the file.
*/
#ifdef DEBUG_BINARY
  int main()
  {
    int i;
    binary* bin1 = convert_num_to_binary(-1, 2);
    binary* bin2 = convert_num_to_binary(+3, 3);
    binary* bin3 = convert_num_to_binary(1, 2);
    binary* bin = NULL; 
    char* temp = NULL;
    
    printf("-1: ");
    for (i = 0; i < bin1->size; i++)
      printf("%d ", bin1->bits[i]);

    printf("\n+3: ");
    for (i = 0; i < bin2->size; i++)
      printf("%d ", bin2->bits[i]);
    
    printf("\n1: ");
    for (i = 0; i < bin3->size; i++)
      printf("%d ", bin3->bits[i]);
    bin = create_byte(bin1, bin2, bin3, NULL);

    printf("\nOverall: ");
    for (i = 0; i < bin->size; i++)
      printf("%d ", bin->bits[i]);
    
    temp = convert_binary_to_base_four(bin);
    printf("\n%s\n", temp);
    free_binary(bin1);
    free_binary(bin2);
    free_binary(bin3);
    free_binary(bin);
    free(temp);
    return 0;
  }
#endif
