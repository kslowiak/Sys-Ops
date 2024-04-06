#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


main(){

				/* id segmentu pamieci dzielonej*/
	int shmID;
				/* taka bedzie struktura danych segmentu */

	char *dane_ptr;
	
	key_t klucz;
	klucz = ftok(".",'H');


				/* utworzenie segmentu pamieci dzielonej z
				   prawami dostepu rw-r----- */

	shmID=shmget(klucz,4,IPC_CREAT | SHM_R  | SHM_W  | SHM_R >> 3);

				/* czy udalo sie utworzyc pamiec dzielona? */ 
	if(shmID == -1){
		perror("Blad utworzenia pamieci dzielonej");
		return 1;
	}

	printf("Identyfikator pamieci dzielonej: %d\n",shmID);
	sleep(1);

				/* dolaczenie segmentu pamieci dzielonej i
				   rzutowanie wskaznika na "nasz" typ danych
				   przechowywanych w pamieci dzielonej
				   (struktura typu Dane) */

	dane_ptr = (char *) shmat(shmID, 0, 0);


				/* czy dolaczenie sie powiodlo? */
				
	if(((long)dane_ptr)==-1){ 
		perror("Blad dolaczenia");
		return 2;
	}

	printf("Dolaczono strukture danych do obszaru pamieci dzielonej\n");
	sleep(1);


				/* zapisanie danych do segmentu pamieci
				   dzielonej */
	strcpy(dane_ptr,"Ala");
//	dane_ptr = "Ala";
	printf("Zapisano w pamieci dzilonej tekst 'Ala'\n");	

	sleep(1);

				/* odlaczenie segmentu pamieci dzielonej i
				   sprawdzenie czy to sie udalo */

	if(shmdt((char *) dane_ptr) == -1) {
		perror("Blad przy odlaczaniu pamieci dzielonej");
		return 3;
	}
	
	printf("Pamiec dzielona odlaczona\n");
	
	return 0;
}
