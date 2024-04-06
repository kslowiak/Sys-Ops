#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>


int main(int argc, char * argv [])
{

   int Sock;

//domyslny port i interfejs sieciowy na ktorym bedzie nasluchiwal serwer jesli nie zostana te wartosci podane przy uruchamianiu serwera.

   int Server_Port = 12900;
   char * Server_IP = "0.0.0.0";

   int nbytes = 0;
   char buf [65536];
   int koniec = 0;
   struct sockaddr_in add;
   struct sockaddr_in from;
   int adl = sizeof(from);

   printf("You can run this program as follows: %s [Server_Port [Server_IP]]\n", argv[0]);

// Utworzenie gniazda

   if((Sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))<0) {
      perror("Error. Socket not created.");
      return 1;
   }
   printf("Socket created. Descriptor: %d\n", Sock);

// Pobranie interfejsu sieciowego i portu na ktorym bedzie nasluchiwal serwer.

   if(argc>1) Server_Port = atoi(argv[1]);
   if(argc>2) Server_IP = argv[2];

// Dowiazanie gniazda na interfejs sieciowy i port na ktorym bedzie nasluchiwal serwer.

   memset(&add, 0, sizeof(struct sockaddr_in) );
   add.sin_family = AF_INET;
   add.sin_addr.s_addr = inet_addr(Server_IP);
   add.sin_port = htons((u_short) Server_Port);

   if(bind(Sock, (struct sockaddr*) &add, sizeof(add))<0) {
      perror("Error. Socket not bound.");
      close(Sock);
      return 1;
   }
   printf("Socket is bound\n");

//Oczekiwanie na dane

   while(! koniec) {
      memset(buf, 0, 65536);
      memset(&from, 0, sizeof(struct sockaddr_in));
      from.sin_family = AF_INET;
      from.sin_addr.s_addr = htonl(INADDR_ANY);
      from.sin_port = htons((u_short) 0);

//Odbierz dane jesli dostepne

      nbytes = recvfrom( Sock, buf, 65536, 0, (struct sockaddr*) &from, &adl );
      if(nbytes>0) {

         printf( "Received req from Sock %s:%d (%d bytes)\n", inet_ntoa( from.sin_addr ), ntohs(from.sin_port), nbytes );
         printf( "Data: \n%s\n", buf );
      }


   }

   close(Sock);
   return 0;
}

