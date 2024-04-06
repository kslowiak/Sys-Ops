/* Przyjeto konwencje ze semafor opuszczony ma wartosc =0, a podniesiony >0 */

#include "sem_wrong.h"

int main(int argc, char **argv)
{
        int	semid;
	int	pid=getpid();

	semid=Tworz(1);

	while(1){
		printf("Proces %d: Chce wejsc do sekcji kryrtycznej.\n",pid);
		P(semid,0);
		printf("Proces %d: Jestem w sekcji krytycznej. Bede tu 5 sekund.\n",pid);
		sleep(5);
		V(semid,0);
		printf("Proces %d: Wyszedlem z sekcji krytycznej. Za chwile znow sprobuje wejsc.\n\n",pid);
		sleep(1);
	}
	exit(0);
}

