#include <stdio.h>
#include <unistd.h>
#include "sem1.h"

int main(){
    int i;
    int j;
    int semid = Tworz(2); // Utwórz dwa semafory

    for(i=2;i<20;i=i+2){
        P(semid, 1); // Opuszczamy semafor 1

        /*poczatek sekcji krytycznej */
        for(j=0;j<i;j++){
            printf("%d ",i);
            fflush(stdout);
            sleep(1);
        }
        printf("\n");
        /*koniec sekcji krytycznej */

        V(semid, 0); // Podnosimy semafor 0
    }
    return 0;
}