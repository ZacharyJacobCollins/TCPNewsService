#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <time.h>
#include "DieWithError.h"


#define RCVBUFSIZE 4200/* Size of receive buffer */

void DieWithError(char *errorMessage);  /* Error handling function */

int main(int argc, char *argv[])
{
    int sock;                        /* Socket descriptor */
    int option;
    struct sockaddr_in echoServAddr; /* Echo server address */
    unsigned short echoServPort;     /* Echo server port */
    char *servIP;                    /* Server IP address (dotted quad) */
    char *echoString;                /* String to send to echo server */
    char echoBuffer[RCVBUFSIZE];     /* Buffer for echo string */
    unsigned int echoStringLen;      /* Length of string to echo */
    int bytesRcvd, totalBytesRcvd;   /* Bytes read in single recv()
                                        and total bytes read */
    
    if ((argc < 2) || (argc > 3))    /* Test for correct number of arguments */
    {
       fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n",
               argv[0]);
       exit(1);
    }
    
    servIP = argv[1];             /* First arg: server IP address (dotted quad) */
    echoString = argv[2];         /* Initialize echo string with garbo */
    
    if (argc == 3){
        echoServPort = atoi(argv[2]); /* Use given port, if any */
    }
    else {
        echoServPort = 7;  /* 7 is the well-known port for the echo service */
    }
    
    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        DieWithError("socket() failed");
    }
    
    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
    echoServAddr.sin_family      = AF_INET;             /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    echoServAddr.sin_port        = htons(echoServPort); /* Server port */


    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0) {
        DieWithError("connect() failed");
    }

    printf("| 1 |  Get all news\n");
    printf("| 2 |  Search for a keyword in 04-09-2017 news\n");
    printf("| 3 |  Exit client\n");
    printf("Select an option: ");
    scanf("%d",&option);
    
    if(option == 1)
    {
        strcpy(echoString, "allnews");
        echoStringLen = strlen(echoString);          /* Determine input length */

        /* Send the string to the server */
        if (send(sock, echoString, echoStringLen, 0) != echoStringLen) {
            DieWithError("send() sent a different number of bytes than expected");
        }
        
        /* Receive the same string back from the server */
        totalBytesRcvd = 0;
        printf("Received: ");                /* Setup to print the echoed string */
        for(;;)
        {
            /* Receive up to the buffer size (minus 1 to leave space for
               a null terminator) bytes from the sender */
            if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0) {
                printf("\n Enjoy your news!");
                break;
            }
            totalBytesRcvd += bytesRcvd;   /* Keep tally of total bytes */
            echoBuffer[bytesRcvd] = '\0';  /* Terminate the string  */
            printf("%s", echoBuffer);      /* Print the echo buffer */
        }   
        exit(0);
    }
    else if(option == 2)
    {
        printf("Enter a search term: ");
        scanf("%s", echoString);

        echoStringLen = strlen(echoString);          /* Determine input length */

        /* Send the string to the server */
        if (send(sock, echoString, echoStringLen, 0) != echoStringLen) {
            DieWithError("send() sent a different number of bytes than expected");
        }
        
        /* Receive the same string back from the server */
        totalBytesRcvd = 0;
        printf("Received: ");                /* Setup to print the echoed string */
        for(;;)
        {
            /* Receive up to the buffer size (minus 1 to leave space for
               a null terminator) bytes from the sender */
            if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0) {
                printf("done");
                break;
            }
            totalBytesRcvd += bytesRcvd;   /* Keep tally of total bytes */
            echoBuffer[bytesRcvd] = '\0';  /* Terminate the string  */
            printf("%s", echoBuffer);      /* Print the echo buffer */
        }
        printf("\n");    /* Print a final linefeed */
        exit(0);
    }
    else
    {
        exit(0);
    }

    close(sock);
    exit(0);
}
