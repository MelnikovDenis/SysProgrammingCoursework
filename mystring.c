#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LINKEDBUFFER_SIZE 256

char *concat(char *s1, const char *s2)
{
      const size_t s1_len = strlen(s1);
      const size_t s2_len = strlen(s2);
      const size_t new_s1_len = s1_len + s2_len + 1;

      printf("\tconcat - длина строки1: %li\n", s1_len + 1);
      printf("\tconcat - длина строки2: %li\n", s2_len + 1);
      printf("\tconcat - длина строки3: %li\n", new_s1_len);

      s1 = realloc(s1, new_s1_len);
        
      memcpy(s1 + s1_len, s2, s2_len + 1);
      
      return s1;
}

struct SLinkedBuffer
{
      char buffer[LINKEDBUFFER_SIZE];
      int idx;
      struct SLinkedBuffer *next;
};

typedef struct SLinkedBuffer LinkedBuffer;
LinkedBuffer *newLinkedBuffer()
{
      LinkedBuffer *result = (LinkedBuffer *) malloc(sizeof(LinkedBuffer));
      if (result == NULL)
      {
            printf("Error while allocating memory!\n");
            exit(1);
      }
      result->idx = 0;
      result->next = NULL;
      return result;
}

char *readLine()
{
      char *result = NULL;
      size_t stringSize = 0;
      
      /* Read into linked buffers */
      LinkedBuffer *baseLinkedBuffer = newLinkedBuffer();
      LinkedBuffer *currentLinkedBuffer = baseLinkedBuffer;
      int currentChar;
      while ((currentChar = fgetc(stdin)) != EOF && currentChar != '\n')
      {
            if (currentLinkedBuffer->idx >= LINKEDBUFFER_SIZE)
            {
                  currentLinkedBuffer->next = newLinkedBuffer();
                  currentLinkedBuffer = currentLinkedBuffer->next;
            }
            currentLinkedBuffer->buffer[currentLinkedBuffer->idx++] = currentChar;
            stringSize++;
      }
      
      /* Copy to a consecutive string */
      int stringIndex = 0;
      result = malloc(sizeof(char) * (stringSize + 1));
      if (result == NULL)
      {
            printf("Error while allocating memory!\n");
            exit(1);
      }
      currentLinkedBuffer = baseLinkedBuffer;
      while (currentLinkedBuffer != NULL)
      {
            for (int i = 0; i < currentLinkedBuffer->idx; i++)
                  result[stringIndex++] = currentLinkedBuffer->buffer[i];
            currentLinkedBuffer = currentLinkedBuffer->next;
      }
      result[stringIndex++] = '\0';
      
      /* Free linked buffers memory */
      while (baseLinkedBuffer != NULL)
      {
            currentLinkedBuffer = baseLinkedBuffer->next;
            free(baseLinkedBuffer);
            baseLinkedBuffer = currentLinkedBuffer;
      }
      
      return result;
}