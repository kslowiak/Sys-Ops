#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int childReaderSock;
void siginthndl( int sig )
{
   printf( "Child reader closing...\n" );
   close( childReaderSock );
   exit(0);
}

int readinchild( int fd )
{
   char ptr[ 1024 ];
   int f, i;
   int nread=1;

   f = fork();
   if( f == 0 ) {

     childReaderSock = fd;
     signal( SIGINT, siginthndl );
     printf( "Child response reader started\n" );
     while( nread > 0 ) {
        nread = recv( fd, ptr, 1024, 0 );
        for( i=0; i<nread; i++ ) {
           printf("%c", ptr[i]);
        }
        fflush( stdout );
     }
     close( fd );
     exit( 0 );
   }
   return f;
}


int main( int argc, char * argv [] )
{
   int Sock;

//Domyslnie adres serwera to 127.0.0.1:13333. Mozna podac wlasny w argumentach wywolania

   int LocalPort = 0;
   int Server_Port = 13333;
   char * Server_IP = "127.0.0.1";

   char buf [1024];
   int koniec = 0;
   struct sockaddr_in srvadd;
   int i, n, reader;

   printf( "You can use this program as follows: %s [Server_IP [Server_PORT]]\n", argv[0] );

//Utworzenie gniazda

   if( ( Sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP) ) < 0 ) {
      perror( "Can't create socket Sock" );
      return -1;
   }
   printf( "Socket created, handle=%d\n", Sock );

   if( argc > 1 ) Server_IP = argv[1];
   if( argc > 2 ) Server_Port = atoi( argv[2] );

   printf( "Sock will communicate with %s:%d\n", Server_IP, Server_Port );

//Przygotowanie struktury danych zawierajacej adres serwera

   memset(&srvadd, 0, sizeof(struct sockaddr_in) );
   srvadd.sin_family = AF_INET;
   srvadd.sin_addr.s_addr = inet_addr(Server_IP);
   srvadd.sin_port = htons((u_short) Server_Port);

//nawiazanie polaczenia z serwerem

   if( connect( Sock, (struct sockaddr *) &srvadd, sizeof(srvadd) ) < 0 ) {
      perror( "Can't connect to remote host" );
      return -1;
   }

//Utorzenie procesu potomnego, ktory bedzie rownolegle mogl odbierac odpowiedzi od serwera. Obsluguje to funkcja readinchild zdefiniowana wyzej

   reader = readinchild( Sock );
   sleep(1);
   while(! koniec) {
     printf( "Enter data strings to send: " );
     fgets( buf, 1024, stdin );
     if( !strncmp( "quit", buf, 4 ) ) break;
     write( Sock, buf, strlen(buf) );
   }

//Procedury zamykania gniazda

   buf[0] = 26;
   buf[1] = 10;
   write( Sock, buf, 2 );
   kill( reader, SIGINT );
   close( Sock );
   wait(0);
   printf( "Sender closing...\n" );
   return 0;
}

