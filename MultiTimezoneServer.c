//
//  MultiTimezoneServer
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

void connection(int FD)
{
    char outputBuf[512];
    char inputBuf[512];
    time_t currentTime;
    char *timeString;
    char timezone[4];
    int state;
    int offset=0;
    ssize_t n;

    time(&currentTime);
    timeString = ctime(&currentTime);
    timeString[strlen(timeString)-1]=0;
    sprintf(outputBuf, "Time is %s - GMT\r\n", timeString);
    write(FD, outputBuf, strlen(outputBuf));

    do
    {
        n = read(FD, inputBuf, 510);
        if(n <= 0)
		{
			fprintf(stderr, "read error\n %d",n);
			return;
		}
		inputBuf[n] = 0;

         if(0 == strcasecmp(inputBuf, "PST\r\n"))
            {
                offset= -8*3600;
                strcpy(timezone,"PST");
            }

            else if(0 == strcasecmp(inputBuf, "MST\r\n"))
            {
                offset= -7*3600;
                strcpy(timezone,"MST");
            }


           else if(0 == strcasecmp(inputBuf, "CST\r\n"))
            {
                offset= -6*3600;
                strcpy(timezone,"CST");
            }

            else if(0 == strcasecmp(inputBuf, "EST\r\n"))
            {
                offset= -5*3600;
                strcpy(timezone,"EST");
            }

            else if(0 == strcasecmp(inputBuf, "GMT\r\n"))
            {
                offset= 0*3600;
                strcpy(timezone,"GMT");
            }

            else if(0 == strcasecmp(inputBuf, "CET\r\n"))
            {
                offset= 1*3600;
                strcpy(timezone,"CET");
            }

            else if(0 == strcasecmp(inputBuf, "MSK\r\n"))
            {
                offset= 3*3600;
                strcpy(timezone,"MSK");
            }

             else if(0 == strcasecmp(inputBuf, "JST\r\n"))
            {
                offset= 9*3600;
                strcpy(timezone,"JST");
            }


            else if(0 == strcasecmp(inputBuf, "AEST\r\n"))
            {
                offset= 10*3600;
                strcpy(timezone,"AEST");
            }

            else if(0 == strcasecmp(inputBuf, "CLOSE\r\n"))
            {

                offset=-1;
            }




    if (offset!=-1)
    {
        time(&currentTime);
        currentTime=currentTime +offset;
        timeString = ctime(&currentTime);
        timeString[strlen(timeString)-1]=0;
        sprintf(outputBuf, "Time is %s - %s\r\n", timeString,timezone);
        write(FD, outputBuf, strlen(outputBuf));
    }

  }while(offset!=-1);


        close(FD);
    //state=sending;

}


int main(int argc, const char * argv[])
{
    int serverSock, clientCon;
    struct sockaddr_in server;
    struct sockaddr_in clientAdr;
    unsigned int client;

    printf("Listening for connections on port 1414\n");

    serverSock=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    server.sin_family= AF_INET;
    server.sin_port=htons(1414);
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
