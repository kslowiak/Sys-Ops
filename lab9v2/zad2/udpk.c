#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>


int Sock;
int LocalPort = 0;
char * LocalIP = "149.156.203.6";
int RemPort = 13333;
char * RemIP = "127.0.0.1";


int main(int argc, char * argv [])
{
   char buf [1024];
   int koniec = 0;
   struct sockaddr_in add;

   printf("You can run this program as follows: %s [server_IP [server_PORT]]\n", argv[0]);

// Utworzenie gniazda 

   if((Sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))<0) {
      perror("Error. Socket not created.");
      return 1;
   }
   printf("Socket created. Descriptor: %d\n", Sock );

// Dowiazanie gniazda na interfejs sieciowy LocalIP=149.156.203.3 i port wybrany przez system (LocalPort=0).

   memset(&add, 0, sizeof(struct sockaddr_in) );
   add.sin_family = AF_INET;
   add.sin_addr.s_addr = inet_addr(LocalIP);
   add.sin_port = htons((u_short) LocalPort);

   if(bind( Sock, (struct sockaddr*) &add, sizeof(add))<0) {
      perror("Error. Socket not bound.");
      close(Sock);
      return 1;
   }
   printf("Socket is bound\n");

// Pobranie adresu i portu serwera z argumentow wywolania.

   if(argc > 1) RemIP = argv[1];
   if(argc > 2) RemPort = atoi(argv[2]);

   printf("Sock will communicate with %s:%d\n", RemIP, RemPort);

   while(! koniec) {
     memset(buf, 0, 1024);
     printf("Enter data string to send: ");
     fgets(buf,1024,stdin);
   
     memset(&add, 0, sizeof(struct sockaddr_in));
     add.sin_family = AF_INET;
     add.sin_addr.s_addr = inet_addr(RemIP);
     add.sin_port = htons((u_short) RemPort);

// Wyslij do serwera

     sendto(Sock, buf, strlen(buf), 0, (struct sockaddr*) &add, sizeof(add) );
     if(!strncmp( "quit", buf, 4 )) koniec = 1;
   }

   close(Sock);
   return 0;
}

