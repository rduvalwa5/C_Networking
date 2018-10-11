/*
 * UnixServer.c
 * Tutorial:  https://www.codeproject.com/Articles/586000/Networking-and-Socket-programming-tutorial-in-C
 *
 *  Created on: Oct 9, 2018
 *
 *	OSXAir:~ rduvalwa2$ lsof | grep 50005
 * NetworkSe 1171 rduvalwa2    3u     IPv4 0xd483a2efcbfea983        0t0        TCP *:50005 (LISTEN)
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

int main(void)
{
  int listenfd = 0,connfd = 0;

  struct sockaddr_in serv_addr;

  char sendBuff[1025];
  int numrv;

/*	a.  a socket function needs to be declared to get the socket descriptor.
		int socket(int domain, int type, int protocol) */

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  printf("socket retrieve success\n");
  printf("Listening on port %s\n", "50005");

  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(sendBuff, '0', sizeof(sendBuff));
/*decide which struct needs to be used based on what domain is used above.
  	2.	AF_INET
			struct sockaddr_in
			{
				short int  sin_family ;
				int        sin_port;
				struct in_addr sin_addr;
			};
		Use struct sockaddr_in if you are using AF_INT on your domain.
 */
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(50005);



  bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));

  if(listen(listenfd, 10) == -1){
      printf("Failed to listen\n");
      return -1;}

  while(1)
    {
      connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL); // accept awaiting request

      strcpy(sendBuff, "Message from server");
      write(connfd, sendBuff, strlen(sendBuff));

      close(connfd);
      sleep(1);
    }

  return 0;
}
