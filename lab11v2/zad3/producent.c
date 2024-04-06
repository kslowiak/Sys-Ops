// prod.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <semaphore.h>
#include "sem1.h"

#define SHM_SIZE 1024
sem_t semaphore;

struct SharedMemory {
    int data;
    char text[256];
};

int main(){
    int semid = Tworz(2); // Utwórz dwa semafory

    key_t key = ftok("shared_memory_key", 65);
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    struct SharedMemory *shared_memory = (struct SharedMemory *)shmat(shmid, NULL, 0);
    int i;
    for(i=1;i<1000;i++){
        if(i!=1){
            P(semid, 0);
        }

        // Poczatek sekcji krytycznej
        printf("Producent: Podaj liczbe calkowita: ");
        scanf("%d", &(shared_memory->data));
        printf("Producent: Podaj tekst: ");
        scanf("%s", shared_memory->text);
        // Koniec sekcji krytycznej
        printf("Producent: Zapisano dane.\n");

        V(semid, 1); // Podnosimy semafor 1
    }

    // Zniszcz semafor
    shmdt(shared_memory);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}