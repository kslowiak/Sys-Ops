// kons.c
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
    int shmid = shmget(key, SHM_SIZE, 0666);

    struct SharedMemory *shared_memory = (struct SharedMemory *)shmat(shmid, NULL, 0);

    while(1){
        P(semid, 1); // Opuszczamy semafor 1

        // Początek sekcji krytycznej
        printf("Konsument: Odczytano dane - Liczba: %d, Tekst: %s\n", shared_memory->data, shared_memory->text);
        // Koniec sekcji krytycznej

        V(semid, 0); // Podnosimy semafor 0
    }

    // Zniszcz semafor
    shmdt(shared_memory);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}