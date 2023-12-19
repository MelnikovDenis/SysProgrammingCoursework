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
    char *str; //string to send

    while (1)
    {        
        printf("%s", "Input string to send: ");
        str = readLine();

        fd = openWriteOnlyFifo(myfifo);
        writeString(fd, str);
        close(fd);

        fd = openReadOnlyFifo(myfifo);
        str = readString(fd, str);

        
        printf("Received string: %s\n\n", str);

        close(fd);
        free(str);
    }
    return 0;
}

