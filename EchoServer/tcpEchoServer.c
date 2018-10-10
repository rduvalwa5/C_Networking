/*
 * tcpEchoServer.c
 *
 *  Created on: Apr 1, 2010
 *      Author: rduvalwa2
 */
#include <stdio.h>
#include <stdlib.h> //for atoi()
#include <sys/socket.h> //for socket() and bind() and connect()
#include <arpa/inet.h> //for sockaddr_in and inet_ntoa()
#include <string.h>  // for memset() places NULL at end string
#include <unistd.h> // for close()

#define MAXPENDING 5 // maximum queued connections
#define RCVBUFSIZE 32  // Size of receive buffer


void DieWithError(char *errorMessage);  //template for function handling errors
void HandleTCPClient(int clntSocket);  // function handling TCP client

int main(int argc, char *argv[])
{
	int servSock;
	int clntSock;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in echoClntAddr;
	unsigned short echoServPort;
	unsigned int clntLen;  // Length of client address

	if (argc !=2)
	{
	 fprintf(stderr, "Usage: %s <server port>\n", argv[0]);
	 exit(1);
	}

	echoServPort = atoi(argv[1]);

	//Create socket for incoming connections
	if((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
	  DieWithError("Incoming socket() failed");
	}

	//Construct local address
	memset(&echoServAddr, 0, sizeof(echoServAddr)); //Zero out structure
	echoServAddr.sin_family = AF_INET;  // Internet Address family
	echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Any incoming interface
    echoServAddr.sin_port = htons(echoServPort);  // Local port

	// Bind to the local address
    if (bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
		DieWithError("bind() failed");

// Mark the socket so it will listen for incoming connections
	if(listen(servSock, MAXPENDING) < 0)
		DieWithError("listen() failed");

for(;;) // run forever
{
	//Set the size of the in-out parameter
	clntLen = sizeof(echoClntAddr);

    // Wait for a client to connect
	if((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)
		DieWithError("accept() failed");

// clntSock is connected to a client

printf("Handing client %s\n", inet_ntoa(echoClntAddr.sin_addr));

HandleTCPClient(clntSock);
}
//Not reached
}

void DieWithError(char *errorMessage)
  {
		  perror(errorMessage);
		  exit(1);
	}

void HandleTCPClient(int clntSocket)
{
		char echoBuffer[RCVBUFSIZE];
		int recvMsgSize;

		// Receive message from client
		if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
						DieWithError("recv() failed");

		//Send recieved string  and receive again until end of transmission
	    while (recvMsgSize > 0)
		{
		  if (send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
		  		DieWithError("send() failed");

		// See if there is more data to receive
		  if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
		     DieWithError("recv() failed");
		}
	close(clntSocket);
}




