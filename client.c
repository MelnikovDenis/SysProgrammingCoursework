#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "erproc.h"

#define PORT 10000
#define IP_ADDR "127.0.0.1"
#define BUF_LEN 256
#define STR_LEN 2048

int main()
{
    struct sockaddr_in adr = { 0 };
    adr.sin_family = AF_INET; //ipv4
    adr.sin_port = htons(PORT);
    adr.sin_addr.s_addr = inet_addr(IP_ADDR);

    char str[STR_LEN] = "";

    while(1)
    {
        char buf[BUF_LEN];

        printf("%s", "Input string: ");
        scanf("%s", buf);
         
        int clientFd = Socket(AF_INET, SOCK_STREAM, 0);
        Connect(clientFd, (struct sockaddr *) &adr, sizeof(adr));

        Write(clientFd, buf, BUF_LEN);

        Read(clientFd, str, STR_LEN);

        printf("Recieved string: %s\n", str);

        close(clientFd);
    }
    return 0;
}