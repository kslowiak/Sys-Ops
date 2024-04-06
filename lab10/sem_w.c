/* Przyjeto konwencje ze semafor opuszczony ma wartosc >0, a podniesiony =0 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int errno;

/*operacje dla funkcji semop */

struct sembuf	op_wait_down[2] = { 0,  0, 0,
				    0,  1, 0 };		/*czekaj az ktos podniesie semafor (otworzy) a potem opusc */
struct sembuf	op_up[1] = { 0, -1, 0 };		/* podnies semafor (otworz) */

int main(int argc, char **argv)
{
        int	semid;
        int	status;
        key_t	key;

        union semun {
                int val;
                struct semid_ds *buf;
                unsigned short *array;
        } arg ;


        key = ftok( ".", 'A'); /*wygenerowanie klucza na podstawie sciezki do pliku "." i znaku "A"*/

/* Wartosci poczatkowe semaforow ustawia zwykle tylko proces, ktory je tworzy. Dolaczajace sie procesy tego nie robia, gdyz moglyby zaklocic w ten sposob prace innych, juz zsynchronizowanych procesow.
Dlatego najpierw uzyjemy funkcji semget bez flagi IPC_CREAT aby sprobowac dolaczyc sie do istniejacego zbioru semaforow, a jesli sie to nie powiedzie, bo taki zbior nie istnieje (blad ENOENT), to go utworzymy funkcja semget juz z IPC_CREAT.

Jesli wartoscia poczatkowa semafora ma byc 0 to w wiekszosci systemow nie trzeba jej explicite ustawiac, gdyz w momencie tworzenia semafory otrzymuja taka wasnie wartosc. Wowczas nie trzeba robic ponizszej "kombinacji", wystarczy jedno wywolanie funkcji semget z flaga IPC_CREAT. */ 

        if ((semid = semget(key, 1, 0640 )) < 0) {
		if(errno==ENOENT) {
			if((semid = semget(key, 1, 0640 | IPC_CREAT | IPC_EXCL))<0) {
				perror("semget error");
				exit(2);
			}
			arg.val=1;
			semctl(semid,0,SETVAL,arg);	/*ustawienie wartosci poczatkowej semafora na stan opuszczony*/
			printf("Proces %s, PID: %d. Utworzylem zbior semaforow o identyfikatorze %d.\n",argv[0],getpid(),semid);
		} else {
			perror("semget error");
			exit(3);
		}
	} else printf("Proces %s, PID: %d. Dolaczylem sie do zbioru semaforow o identyfikatorze %d.\n",argv[0],getpid(),semid);

	printf("Chce teraz wejsc do sekcji krytycznej.\n");
	if (semop(semid, op_wait_down, 2) < 0)		/*To jest realizacja funkcji P()*/
		perror("semop down error");
	else
		printf("Proces %s, PID: %d. Doczekalem sie na podniesienie semafora (wartosc 0) i natychmiast go opuscilem (ustawilem go na 1). Jestem w sekcji krytycznej.\n",argv[0],getpid());

	sleep(2);
	printf("Proces %s, PID: %d. Wychodze z sekcji krytycznej i koncze dzialanie. Zlosliwie nie podnioslem semafora ;)\n",argv[0],getpid());
	exit(0);
}

