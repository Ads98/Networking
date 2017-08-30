//
//  MultiTimezoneClient
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
#define bufferSize 12


int main(int argc, const char * argv[])
{
    int sockFD;
    int con;
    char buffer[bufferSize+1];
    char line[512];
    char temp[512];
    ssize_t n;
    int count;
    int more;
    int pos=2;
    struct  sockaddr_in adstr;
    struct hostent *hp;
    sockFD=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (sockFD>-1)
    {	/* create connection*/
           memset(&adstr, 0,sizeof(adstr));
	       adstr.sin_family = AF_INET;
           adstr.sin_port=htons(kMULTITIMEZONEPORT);

           hp=gethostbyname(argv[1]);
           memcpy(&adstr.sin_addr,hp->h_addr,hp->h_length);

           con=connect(sockFD,(struct sockaddr *)&adstr ,sizeof(adstr));
           if (con>-1)

        {
        pos=2;
        //printf("findinging time\n");
            /*read in data*/
         do
            {
                do
                {
                   // printf("pos = %d\n",pos);
                    more = 1;
                    //printf("reading\n");
                    n = read(sockFD, buffer, bufferSize);
                    //printf("finished reading\n");
                    buffer[n] = '\0';
                    if(n <= 0)
                    break;
                    for(count = 0; count < n; count++)
                    {
                        if(buffer[count] == 10)
                        {
                            more = 0;
                            break;
                        }
                    }
		    if(strcmp(buffer,"BYE\r\n")!=0)
                    printf("%s", buffer);
                } while(more);

                /*if(n <= 0)
                {
                    break;
                }*/


                //  printf("start of loop\n");
          //  printf("[[error check]] argc===== %d \n",argc);

            if (pos< argc)
            {
               // printf("looping");
                strcpy(temp,argv[pos]);
                strcat(temp,"\r\n");
                printf("\n");
                //printf("[temp = %s\n]",temp);
                write(sockFD, temp, strlen(temp));
             }

            else if(pos==argc)
            {
                if(strcasecmp(temp,"CLOSE\r\n")!=0)
                {
                strcpy(temp,"CLOSE");
                strcat(temp,"\r\n");
                printf("\n");
               // printf("[!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!temp = %s]\n",temp);
                write(sockFD, temp, strlen(temp));
                }
            }
          //  printf("here\n");
            pos++;
           // printf("pos == %d\n",pos);

        /*for(pos=1;pos++;pos==argc)
        {
                printf("looping");
                strcpy(temp,argv[pos]);
                strcat(temp,"\r\n");
                printf("\n");
                write(sockFD, temp, strlen(temp));
        }*/

            } while(n > 0);

            close(sockFD);

            return 0;

    }
     else
        {
            printf("ERROR NETWORK CONNECTION FAIL 1");
        }
  }
    else
        {
            printf("ERROR socket defention  FAIL 2");

        }
        return 0;
}












