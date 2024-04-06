#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>

int descr;

void endp(){
	close(descr);
	printf("Program czytelnika zakonczony.\n");
	exit(0);
}

int main(){
	char tab[100];
	int size;

	signal(SIGTERM,endp);
	signal(SIGINT,endp);

	

	descr=open("plik_fifo",O_RDONLY);
	if(descr==-1){
		perror("blad otwarcia pliku fifo");
		exit(0);
	}
	

	printf("Plik: plik_fifo otwarty do czytania\n");
	for(;;){
		bzero(tab,sizeof(tab));
		size=read(descr,tab,100);
		if(size==0) {
			printf("Proces pisarza sie zakonczyl\nZakonczyc dzialanie programu (t/n)?\n");
			switch(fgetc(stdin)){
			case 't': close(descr); exit(0);
			default: fflush(stdin); sleep(1);
			}
		}
		else
		printf("Przeczytano %d znakow.\nOdczytany tekst:\n%s\n",size,tab);
	}
}
