#include "erproc.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int Socket(int domain, int type, int protocol) 
{
    int res = socket(domain, type, protocol);
    if (res == -1) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    return res;
}
void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) 
{
    int res = bind(sockfd, addr, addrlen);
    if (res == -1) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}
void Listen(int sockfd, int backlog) 
{
    int res = listen(sockfd, backlog);
    if (res == -1) 
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) 
{
    int res = accept(sockfd, addr, addrlen);
    if (res == -1) 
    {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    return res;
}

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) 
{
    int res = connect(sockfd, addr, addrlen);
    if (res == -1) 
    {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }
}

void Read(int __fd, void *__buf, size_t __nbytes)
{
    ssize_t nread = read(__fd, __buf, __nbytes);
    if (nread == -1) 
    {
        perror("read failed");
        exit(EXIT_FAILURE);
    }
    else if (nread == 0) 
    {
        printf("EOF occured\n");
    }
}

void Write(int __fd, const void *__buf, size_t __n)
{
    ssize_t nwrite = write(__fd, __buf, __n);
    if (nwrite == -1) 
    {
        perror("write failed");
        exit(EXIT_FAILURE);
    }
}

void Pipe(int *__pipedes)
{
    if(pipe(__pipedes) == -1 | fcntl(__pipedes[0], F_SETFL, O_NONBLOCK) == -1)
    {
        perror("pipe creation failed");
        exit(EXIT_FAILURE);
    }
}