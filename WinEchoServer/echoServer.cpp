/*
 * https://www.binarytides.com/code-tcp-socket-server-winsock/
 *   Bind socket to port 8888 on localhost
 *
 *   In Eclipse go to project > properties > MingW Linker C++ > Libraries
 *   add ws2_32
 *   this puts this library in the linker path
 *   in winsock.h
 *    (gs)etsockopts exported from the WinSock 2 lib, ws2_32.dll.
 *    Refer ws2tcpip.h.
 *
 */
#include<io.h>
#include<stdio.h>
#include<winsock2.h>

//#pragma comment(lib,"ws2_32.lib") //Winsock Library

int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET s , new_socket;
    struct sockaddr_in server , client;
    int c;
    char *message;

    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }

    printf("Initialised.\n");

    //Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }

    printf("Socket created.\n");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    //Bind
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
    }

    puts("Bind done");

    //Listen to incoming connections
    listen(s , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");

    c = sizeof(struct sockaddr_in);
    new_socket = accept(s , (struct sockaddr *)&client, &c);
    if (new_socket == INVALID_SOCKET)
    {
        printf("accept failed with error code : %d" , WSAGetLastError());
    }

    puts("Connection accepted");

    //Reply to client
    message = "Hello Client , I have received your connection. But I have to go now, bye\n";
    send(new_socket , message , strlen(message) , 0);

    getchar();

    closesocket(s);
    WSACleanup();

    return 0;
}

