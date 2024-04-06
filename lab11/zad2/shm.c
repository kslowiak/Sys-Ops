#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

main(){

				/* id segmentu pamieci wspoldzielonej */
	int shmID;
				/* taka bedzie struktura danych segmentu */
	struct Dane {
		int numer;
		int wartosc;
		char napis[13];
	} *dane_ptr;
	
	key_t klucz;
	klucz = ftok(".",'K');


				/* utworzenie segmentu pamieci wspoldzielonej z
				   prawami dostepu rw-r----- */

	shmID=shmget(klucz,sizeof(struct Dane),IPC_CREAT | SHM_R  | SHM_W  | SHM_R >> 3);

				/* czy udalo sie utworzyc pamiec wspoldzielona? */ 
	if(shmID == -1){
		perror("Blad utworzenia pamieci wspoldzielonej");
		return 1;
	}

	printf("Identyfikator pamieci wspoldzielonej: %d\n",shmID);
	sleep(10);

				/* dolaczenie segmentu pamieci wspoldzielonej i
				   rzutowanie wskaznika na "nasz" typ danych
				   przechowywanych w pamieci wspoldzielonej
				   (struktura typu Dane) */

	dane_ptr = (struct Dane *) shmat(shmID, 0, 0);


				/* czy dolaczenie sie powiodlo? */
				
	if(((long)dane_ptr)==-1){ 
		perror("Blad dolaczenia");
		return 2;
	}

	printf("Dolaczono strukture danych do obszaru pamieci wspoldzielonej\n");
	sleep(10);


				/* zapisanie danych do segmentu pamieci
				   wspoldzielonej */
	dane_ptr->numer = 2;
	dane_ptr->wartosc = 5;
	strcpy(dane_ptr->napis,"TEXT");
	

				/* odczyt danych z pamieci wspoldzielonej */

	printf("\nZAWARTOSC PAMIECI WSPOLDZIELONEJ:\nNumer %d\nWartosc %d\nNapis %s\n",dane_ptr->numer,dane_ptr->wartosc,&dane_ptr->napis);
	sleep(10);

				/* odlaczenie segmentu pamieci wspoldzielonej i
				   sprawdzenie czy to sie udalo */

	if(shmdt((char *) dane_ptr) == -1) {
		perror("Blad przy odlaczaniu pamieci wspoldzielonej");
		return 3;
	}
	
	printf("Pamiec wspoldzielona odlaczona\n");
	sleep(10);

				/* skasowanie segmentu pamieci wspoldzielonej i
				   sprawdzenie czy to sie udalo */

	if(shmctl(shmID, IPC_RMID, 0) == -1) {
		perror("Blad przy usuwaniu pamieci wspoldzielonej");
		return 4;
	}
	
	printf("Pamiec wspoldzielona skasowana\n");
	sleep(1);
	
	return 0;
}
