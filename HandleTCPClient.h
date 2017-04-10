#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */

#define RCVBUFSIZE 4096   /* Size of receive buffer */

void DieWithError(char *errorMessage);  /* Error handling function */

void HandleTCPClient(int clntSocket)
{
    FILE *filepointer;                      
    filepointer = fopen("04-09-2017.txt", "r");
    fpos_t firstline;
    fpos_t temp;
    fgetpos(filepointer,&firstline);

    char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
    char searchTerm[RCVBUFSIZE];

    int recvMsgSize;                    /* Size of received message */

    
    /* Receive message from client */
    if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0) {
        DieWithError("recv() failed");
    }
    echoBuffer[recvMsgSize] = '\0';

       /* Store search term in variable */
    strcpy(searchTerm,echoBuffer);

    /* retrieve and send all news */
    if( strcmp(searchTerm, "allnews")==0 ) {
        printf("Retrieving all news\n");

        while (1)
        {
            fgets(echoBuffer,4096,filepointer);
            if(feof(filepointer)|| strlen(echoBuffer)< 1)
            {
                break;  
    /* Send received string and receive again until end of transmission */
    while (recvMsgSize > 0)      /* zero indicates end of transmission */
    {
        /* Echo message back to client */
        if (send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize) {
            DieWithError("send() failed");
        }
        
        /* See if there is more data to receive */
        if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0) {
            DieWithError("recv() failed");
        }
    }
            }
            /* Echo message back to client */
            if(strstr(echoBuffer, "#item"))
            {
                echoBuffer[0] = '#';
                echoBuffer[1] = 'i';
                echoBuffer[2] = 't';
                echoBuffer[3] = 'e';
                echoBuffer[4] = 'm';
                echoBuffer[5] = ':';
                send(clntSocket, echoBuffer, strlen(echoBuffer), 0);
            }
            else
            {
                send(clntSocket, echoBuffer, strlen(echoBuffer), 0);
            }
        }
    /* We're looking for a specific news piece, grab that piece and send it back */
    } else {
       printf("Searching for %s\n", searchTerm);
       while(1)
        {
        fgetpos(filepointer,&temp);
            fgets(echoBuffer, 4096,filepointer);
            if(strstr(echoBuffer, "#item"))
            {
                firstline = temp;
                fgets(echoBuffer, 4096,filepointer);
            }
            if(strstr(echoBuffer, searchTerm))
            {
                fsetpos(filepointer, &firstline);
                fgets(echoBuffer, 4096,filepointer);
                while(strlen(echoBuffer) > 1 && !feof(filepointer))
                {
                    if(strstr(echoBuffer, "#item"))
                    {
                        echoBuffer[0] = 'i';
                        echoBuffer[1] = 't';
                        echoBuffer[2] = 'e';
                        echoBuffer[3] = 'm';
                        echoBuffer[4] = ':';
                        send(clntSocket, echoBuffer, strlen(echoBuffer), 0);
                    }
                    else
                    {
                        send(clntSocket, echoBuffer, strlen(echoBuffer), 0);
                    }
                    fgets(echoBuffer, 4096,filepointer);
                }
            
            }
        if(feof(filepointer))
            {
                break;
            }
        } 
    } 
    
    close(clntSocket);    /* Close client socket */
}