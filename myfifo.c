#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int openWriteOnlyFifo(char *path)
{
      int fd = -1;
      if((fd = open(path, O_WRONLY)) == -1)
      {
            printf("%s", "WriteOnlyFifo open error\n");
            exit(1);
      }
      else
      {
            printf("%s", "WriteOnlyFifo opening successful\n");
      }
      return fd;
}
void writeString(int fd, char *str)
{
      size_t len = strlen(str) + 1; //string to send length
      if (write(fd, &len, sizeof(size_t)) == -1) //write length of string
      {
            printf("%s", "Write length error\n");
            exit(1);
      }
      else
      {
            printf("Writing length successful: %li\n", len); 
      }

      if (write(fd, str, len) == -1) //write string
      {
            printf("%s", "Write string error\n");
            exit(1);
      }
      else
      {
            printf("Writing string successful: %s\n", str);
      }
}
char* readString(int fd, char *str)
{
      size_t len;
      if (read(fd, &len, sizeof(size_t)) == -1) //read length of string
      {                  
            printf("%s", "Read length error\n");
            exit(1);
      }
      else
      {
            printf("Reading length successful: %li\n", len);
      }
      
      if (read(fd, str, len * sizeof(char)) == -1) //read string
      {
            printf("%s", "Read str error\n");
            exit(1);
      }
      else
      {
            printf("Reading string successful: %s\n", str);
      } 
      return str;
}
int openReadOnlyFifo(const char *path)
{
      int fd = -1;
      if((fd = open(path, O_RDONLY)) == -1) //open read only fifo
      {
            printf("%s", "ReadOnlyFifo open error\n");
            exit(1);
      }
      else
      {
            printf("%s", "ReadOnlyFifo opening successful\n");
      }
      return fd;
}
void createFifo(const char *path)
{
      if(mkfifo(path, 0666) == -1) //create fifo
      {
            printf("%s", "Fifo create error\n");
            exit(1);
      }
      else
      {
            printf("%s", "Fifo creation successful\n");
      }
}