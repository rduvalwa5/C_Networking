/*
 * echoClient.c
 *
 *  Created on: Oct 4, 2018
 *      Author: rduvalwa2
 *
 * OSXAir:~ rduvalwa2$ sudo lsof -i -n -P | grep TCP | grep Echo
 * EchoClien 3585      rduvalwa2    3u  IPv4 0x6b2ccc57045731d1      0t0    TCP 127.0.0.1:61266->127.0.0.1:8888 (CLOSE_WAIT)
 * EchoServe 3632      rduvalwa2    3u  IPv4 0x6b2ccc5718f935b1      0t0    TCP *:11306 (LISTEN)
 * EchoServe 3632      rduvalwa2    4u  IPv4 0x6b2ccc57147b71d1      0t0    TCP 127.0.0.1:11306->127.0.0.1:61503 (ESTABLISHED)
 * EchoClien 3659      rduvalwa2    3u  IPv4 0x6b2ccc57195fff11      0t0    TCP 127.0.0.1:61503->127.0.0.1:11306 (ESTABLISHED)
 *
	C ECHO client example using sockets
*/
#include<stdio.h>	//printf
#include<string.h>	//strlen
#include<sys/socket.h>	//socket
#include<arpa/inet.h>	//inet_addr

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
	server.sin_port = htons(789987);

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

	close(sock);
	return 0;
}

