 /*
 * echoClient.cpp
 *
 *  Created on: Oct 4, 2018
 *      Author: rduvalwa2
 *
 * C:\Users\rduvalwa2\Cworkspace\WinEchoclient>tasklist /fi "imagename eq WinEchoServer.exe"
 *
 * Image Name                     PID Session Name        Session#    Mem Usage
 * ========================= ======== ================ =========== ============
 * WinEchoServer.exe             3848 Console                    1      2,204 K
 *
 *	C ECHO client example using sockets
 *	Also, please not that the lib is named "ws2_32"
 *
 *	gcc -o echoClientWin.exe echoClient.o -lws2_32
*/
#include<stdio.h>	//printf
#include<string.h>	//strlen
//#include<sys/socket.h>	//socket
//#include<arpa/inet.h>	//inet_addr
#include<winsock2.h>

int main(int argc , char *argv[])
{
	int sock;
	struct sockaddr_in server;
	char message[1000] , server_reply[2000];

	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 8888 );

	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}

	puts("Connected\n");

	//keep communicating with server
	while(1)
	{
		printf("Enter message : ");
		scanf("%s" , message);

		//Send some data
		if( send(sock , message , strlen(message) , 0) < 0)
		{
			puts("Send failed");
			return 1;
		}

		//Receive a reply from the server
		if( recv(sock , server_reply , 2000 , 0) < 0)
		{
			puts("recv failed");
			break;
		}

		puts("Server reply :");
		puts(server_reply);
	}

//	close(sock);
	return 0;
}
