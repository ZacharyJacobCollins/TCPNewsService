#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */

#define RCVBUFSIZE 4200   /* Size of receive buffer */

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

        for(;;)
        {
            //read from stream
            fgets(echoBuffer,4200,filepointer);
            //Make sure we don't overstep our bounds here check eof
            if(feof(filepointer)|| strlen(echoBuffer)< 1)
            {
                break;  
            }
            if(strcmp(searchTerm, "#item")==0)
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
        for(;;)
        {
        fgetpos(filepointer,&temp);
            fgets(echoBuffer, 4200,filepointer);
            //Find occurence of #item
            if(strstr(echoBuffer, "#item"))
            {
                firstline = temp;
                fgets(echoBuffer, 4200,filepointer);
            }
            //Find occurence of search term
            if(strstr(echoBuffer, searchTerm))
            {
                //stream
                fsetpos(filepointer, &firstline);
                //read from stream
                fgets(echoBuffer, 4200,filepointer);
                //Make sure we don't overstep our bounds here check eof
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
                    fgets(echoBuffer, 4200,filepointer);
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