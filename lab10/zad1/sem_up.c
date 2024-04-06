/* Przyjeto konwencje ze semafor opuszczony ma wartosc >0, a podniesiony =0 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

/*operacje dla funkcji semop */

struct sembuf	op_wait_down[2] = { 0,  0, 0,
				    0,  1, 0 };		/*czekaj az ktos podniesie semafor (otworzy) a potem opusc */
struct sembuf	op_up[1] = { 0, -1, 0 };		/* podnies semafor (otworz) */

int main(int argc, char **argv)
{
        int	semid;
        int	status;
        key_t	key;
	int	vs;

        union semun {
                int val;
                struct semid_ds *buf;
                unsigned short *array;
        } arg ;


        key = ftok( ".", 'A'); /*wygenerowanie klucza na podstawie sciezki do pliku "." i znaku "A"*/

        if ((semid = semget(key, 1, 0640 )) < 0) {
		perror("semget error:");
		exit(2);
	} else printf("Proces %s, PID: %d. Dolaczylem sie do zbioru semaforow o identyfikatorze %d.\n",argv[0],getpid(),semid);

	if ((vs=semctl(semid, 0, GETVAL, arg)) < 0)
                perror("semctl error:");

	switch (vs) {
		case 1: printf("Semafor opuszczony. Podnosze.\n");
			break;
		case 0: printf("Semafor podniesiony. Nie mam nic do roboty\n");
			exit(0);
	}
	if (semop(semid, op_up, 1) < 0)		/*To jest realizacja funkcji V()*/
		perror("semop up error:");
	else
		printf("Podnioslem\n");

	exit(0);
}

