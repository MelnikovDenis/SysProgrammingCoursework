#include <sys/types.h>
#include <sys/socket.h>

int Socket(int domain, int type, int protocol);

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

void Listen(int sockfd, int backlog);

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

void Read(int __fd, void *__buf, size_t __nbytes);

void Write(int __fd, const void *__buf, size_t __n);

void Pipe(int *__pipedes);