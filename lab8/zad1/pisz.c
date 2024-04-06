#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int descr;

void endp(){
	close(descr);
	printf("Program pisarza zakonczony.\n");
	exit(0);
}

void b_p(){
	printf("Padl proces czytelnika. Broken pipe. Koncze.\n");
	close(descr);
	exit(0);
}

int main(){
	char tab[100];
	int size;

	signal(SIGTERM,endp);
	signal(SIGINT,endp);
	signal(SIGPIPE,b_p);

	descr=open("plik_fifo",O_WRONLY);
	if(descr==-1){
		perror("blad otwarcia pliku fifo");
		exit(0);
	}
	printf("Plik: plik_fifo otwarty do pisania\n");
	for(;;){
		printf("Podaj tekst do wyslania do fifo: ");
		scanf("%s",tab);
		size=write(descr,tab,strlen(tab));
		printf("Wpisano %d znakow.\n",size);
	}
}
