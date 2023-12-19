#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "mystring.h"
#include "myfifo.h"

int main()
{
      int fd = -1; //fifo file descriptor      
      char *myfifo = "/tmp/myfifo"; // FIFO file path
      unlink(myfifo); //remove if fifo already exists

      createFifo(myfifo);
      

      char *str = (char*)malloc(sizeof(char));
      char *res = (char*)malloc(sizeof(char));      
      strcpy(str, "");
      strcpy(res, "");
            
      while (1)
      {           
            fd = openReadOnlyFifo(myfifo); 
            str = readString(fd, str);
            close(fd);
            printf("%s", "\n");

            int pipes[2]; //pipe descriptors
            if(pipe(pipes) == -1)
            {
                  printf("%s", "Pipe creation error!\n");
                  return 1;
            }

            if(fork() > 0) //parent
            {
                  close(pipes[1]); //close writing
                  res = readString(pipes[0], res);
                  close(pipes[0]);
            }
            else //child
            {                  
                  close(pipes[0]); //close reading                    
                  res = concat(res, str); 
                  size_t len = strlen(res) + 1;
                  write(pipes[1], &len, sizeof(size_t));
                  write(pipes[1], res, len);
                  close(pipes[1]);

                  free(res);
                  free(str);
                  exit(0);
            }


            fd = openWriteOnlyFifo(myfifo);
            writeString(fd, res);
            close(fd);

            printf("%s", "\n\n");      
      }
      return 0;
}
