#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int descr[2];	/*tablica deskryptorow */

void endp(){
	close(descr[0]);
	printf("Program czytelnika zakonczony.\n");
	exit(0);
}

void endm(){
        close(descr[1]);
        printf("Program pisarza zakonczony.\n");
        exit(0);
}

void b_p(){
        printf("Padl proces czytelnika. Broken pipe. Koncze.\n");
        close(descr[1]);
        exit(0);
}


int main(){
	char tab[100];
	int size;

	if(pipe(descr)<0) perror("Blad !");

	if(fork() == 0){

		close(descr[1]);

		signal(SIGTERM,endp);
        	signal(SIGINT,endp);

		printf("Proces potomny otworzyl potok do czytania\n");
		for(;;){
			bzero(tab,sizeof(tab));
			size=read(descr[0],tab,100);
			if(size==0) endp();
			printf("Przeczytano %d znakow.\nOdczytany tekst:\n%s\n",size,tab);
		}
	}
	else {

		close(descr[0]);

		signal(SIGTERM,endm);
		signal(SIGINT,endm);
		signal(SIGPIPE,b_p);



		printf("Proces macierzysty otworzyl potok do pisania\n");
		for(;;){
			sleep(1);
			printf("Podaj tekst do wyslania do lacza komunikacyjnego: ");
			scanf("%s",tab);
			size=write(descr[1],tab,strlen(tab));
			printf("Wpisano %d znakow.\n",size);
		}



	}
}
