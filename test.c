#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mystring.h"

#define str1 "111111111111111111111111111111111111111111111111111111"
#define str2 "122222222222222222222222222222222222222222222222222222"

int main()
{
      char * test_str1 = (char*)malloc((strlen(str1) + 1) * sizeof(char));      
      strcpy(test_str1, str1);
      char * test_str2 = (char*)malloc((strlen(str2) + 1) * sizeof(char));
      strcpy(test_str2, str2);

      printf("test_str1: %s\n", test_str1);
      printf("test_str2: %s\n", test_str2);
      
      test_str1 = concat(test_str1, test_str2);
      size_t test_str1_len = strlen(test_str1) + 1;

      printf("new_test_str1: %s\n", test_str1);
      printf("new_test_str1_len: %li\n", test_str1_len);

      free(test_str1);
      free(test_str2);

      return 0;
}
