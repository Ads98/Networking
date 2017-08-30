//
//  DaytimeServer
//
//  Created by <insert name>
//  Username: <inser username>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <time.h>

#define kDAYTIMEPORT       1313
#define kMULTITIMEZONEPORT 1414
#define waiting  0
#define sending
#define new
#define fin 4

void connection(int FD)
{
    char outputBuf[512];
    char inputBuf[512];
    time_t currentTime;
    char *timeString;
    int state= waiting;
    ssize_t n;

    time(&currentTime);
    timeString = ctime(&currentTime);
    timeString[strlen(timeString)-1]=0;
    sprintf(outputBuf, "Time is %s - GMT\r\n", timeString);
    write(FD, outputBuf, strlen(outputBuf));
    //state=sending;

}


int main(int argc, const char * argv[])
{
    int serverSock, clientCon;
    struct sockaddr_in server;
    struct sockaddr_in clientAdr;
    unsigned int client;

    printf("Listening for connections on port 1313\n");
    serverSock=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    server.sin_family= AF_INET;
    server.sin_port=htons(1313);
    server.sin_addr.s_addr=INADDR_ANY;
    if (bind(serverSock, (struct sockaddr*)&server, sizeof(server))<0)
    {
        printf("bind failed\n");
        return 1;
    }

    if(listen(serverSock, 20)<0)
    {
        fprintf(stderr, "faiiled to listen\n");
        return 2;
    }
    while (1)
    {
        client= sizeof(clientAdr);
        clientCon=accept(serverSock,(struct sockaddr*)&clientAdr,&client);
        if (clientCon<0)
        {
            fprintf(stderr,"accept failed \n");
            return 3;
        }
        connection(clientCon);
        close(clientCon);
    }

    return 0;
}
