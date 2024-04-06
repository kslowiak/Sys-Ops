/*Przyjeta konwencja: 0 semafor opuszczony. */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int errno;

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
} arg ;

/*************************************************************************/
/* funkcja P opuszcza wybrany semafor (element) ze zbioru semaforowago   */
/*************************************************************************/

void P(int zbior,int element){
	struct sembuf bs = {element,-1,0};
	if(semop(zbior,&bs,1)==-1){
		perror("Blad: funkcja P");
		exit(6);
	}
}

/*************************************************************************/
/* funkcja V podnosi wybrany semafor (element) ze zbioru semaforowago    */
/*************************************************************************/

void V(int zbior,int element){
	struct sembuf bs = {element,1,0};
	if(semop(zbior,&bs,1)==-1){
		perror("Blad: funkcja V");
		exit(7);
	}
}

/******************************************************************************/
/* Funkcja TWORZ tworzy nowy, zbior semaforow na podstawie klucza             */
/* wygenerowanego przez ftok lub dolacza sie do istniejacego zbioru semaforow.*/
/* Liczba semaforow w zbiorze bedzie rowna wartosci liczba_elementow.         */
/* Wszystkie semafory maja ustawiany stan poczatkowy na podniesiony.          */
/******************************************************************************/

int Tworz(int liczba_elementow)
{
	int semid;
	int i;
	int key=0;

	key = ftok( ".", 'B');
	if ((semid = semget(key, liczba_elementow, 0644 )) < 0) {
		if(errno==ENOENT) {
			if((semid = semget(key, liczba_elementow, 0644 | IPC_CREAT | IPC_EXCL))<0) {
				perror("semget error");
				exit(2);
                        }
			arg.val=1;
			for(i=0;i<liczba_elementow;i++) 
				semctl(semid,i,SETVAL,arg);     /*ustawienie wartosci poczatkowej semaforow na stan podniesiony*/
			printf("Proces %d: Utworzylem zbior semaforow o identyfikatorze %d.\n",getpid(),semid);
		} else {
			perror("semget error");
			exit(3);
		}
	} else printf("Proces %d: Dolaczylem sie do zbioru semaforow o identyfikatorze %d.\n",getpid(),semid);
	return(semid);
}


/*************************************************************************/
/* funkcja USUN likwiduje wszystkie semafory                             */
/*************************************************************************/

void Usun(int semid){
	if(semctl(semid, 0, IPC_RMID, 0)<0) {
		perror("Blad: funkcja usun");
		exit(4);
	}
}

