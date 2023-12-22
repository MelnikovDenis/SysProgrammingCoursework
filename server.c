#include "erproc.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define PORT 10000
#define IP_ADDR "127.0.0.1"
#define MAX_CLIENTS 10
#define BUF_LEN 256
#define STR_LEN 2048
#define NEED_SEM 1

union semun 
{
    int val;
    struct semid_ds *buf;
    unsigned short  *array;
};

struct sembuf locker = { 0, -1, SEM_UNDO };
struct sembuf unlocker = { 0, +1, SEM_UNDO };

int main()
{    
    //создание семафора (идентификатор, кол-во семафоров, флаги)
    int semId = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    if(semId == -1)
    {
        perror("semget error");
        exit(EXIT_FAILURE);
    }
    union semun u;
    u.val = 1;
    //переводим семафор в состояние готовности (идентификатор, номер семафора, действие, объединение)
    if(semctl(semId, 0, SETVAL, u) == -1)
    {
        perror("semctl error");
        exit(EXIT_FAILURE);
    }

    int serverFd = Socket(AF_INET, SOCK_STREAM, 0); //ipv4, tcp, default
    
    struct sockaddr_in adr = { 0 };
    adr.sin_family = AF_INET; //ipv4
    adr.sin_port = htons(PORT);
    adr.sin_addr.s_addr = inet_addr(IP_ADDR);

    Bind(serverFd, (struct sockaddr *) &adr, sizeof(adr));

    Listen(serverFd, MAX_CLIENTS);

    char str[STR_LEN] = "";

    int pipes[2]; //pipe descriptors
    Pipe(pipes);

    printf("Start listening %s on port: %i\n", IP_ADDR, PORT);
    
    while(1)
    {        
        socklen_t adrlen = sizeof(adr);
        
        int clientFd = Accept(serverFd, (struct sockaddr *) &adr, &adrlen);

        //вход в критическую секцию
        if(NEED_SEM)
            semop(semId, &locker, 1);

        //неблокирующее чтение (блоками по 256 байт)
        char buf[BUF_LEN];
        size_t nread = read(pipes[0], buf, BUF_LEN);
        switch(nread)
        {
            case -1:
                if(errno == EAGAIN)
                    break;
                else{
                    perror("read failed");
                    exit(EXIT_FAILURE);
                }
            case 0:
                printf("EOF occured\n");
                break;
            default:               
                strcat(str, buf);
                while(read(pipes[0], buf, BUF_LEN) > 0)
                {
                    strcat(str, buf);
                }                
                break;
        }

        //выход из критической секции
        if(NEED_SEM)
            semop(semId, &unlocker, 1);

        if (fork() == 0) //child
        {            
            printf("%s", "\tStart child proccess\n");

            close(pipes[0]); //close reading

            //вход в критическую секцию
            if(NEED_SEM)
                semop(semId, &locker, 1);
                                
            Read(clientFd, buf, BUF_LEN);
            printf("\tReceived from socket string: %s\n", buf);

            sleep(5);

            strcat(str, buf); 
            printf("\tConcated string: %s\n", str);
                        
            Write(clientFd, str, STR_LEN);
            Write(pipes[1], buf, BUF_LEN);

            //выход из критической секции
            if(NEED_SEM)
                semop(semId, &unlocker, 1);

            close(clientFd);
            close(pipes[1]);
            printf("%s", "\tEnd child proccess\n\n");
            exit(EXIT_SUCCESS);            
        }
    }
    
    close(pipes[1]); //close writing
    close(pipes[0]); //close reading
    close(serverFd); 
    return EXIT_SUCCESS;
}