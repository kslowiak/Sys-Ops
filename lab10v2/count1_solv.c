#include <stdio.h>
#include <unistd.h>
#include "sem1.h"

int main(){
    int i;
    int j;
    int semid = Tworz(2); // Utwórz dwa semafory

    for(i=1;i<20;i=i+2){
        if(i!=1){
            P(semid, 0);
        }

        /*poczatek sekcji krytycznej */
        for(j=0;j<i;j++){
            printf("%d ",i);
            fflush(stdout);
        }
        sleep(1);
        printf("\n");
        /* koniec sekcji krytycznej */

        V(semid, 1); // Podnosimy semafor 1
    }
    return 0;
}