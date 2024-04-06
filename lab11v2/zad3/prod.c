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

#define SHM_SIZE 1024
sem_t semaphore;

struct SharedMemory {
    int data;
    char text[256];
};

int main() {
    // Inicjalizacja semafora
    sem_init(&semaphore, 1, 1);

    key_t key = ftok("shared_memory_key", 65);
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    struct SharedMemory *shared_memory = (struct SharedMemory *)shmat(shmid, NULL, 0);

    while (1) {
        sem_wait(&semaphore);  // Czekaj na dostęp do sekcji krytycznej
        // Poczatek sekcji krytycznej
        printf("Producent: Podaj liczbe calkowita: ");
        scanf("%d", &(shared_memory->data));
        printf("Producent: Podaj tekst: ");
        scanf("%s", shared_memory->text);
        // Koniec sekcji krytycznej
        printf("Producent: Zapisano dane.\n");
        sem_post(&semaphore);  // Zwolnij dostęp do sekcji krytycznej
    }

    // Zniszcz semafor
    shmdt(shared_memory);
    shmctl(shmid, IPC_RMID, NULL);
    sem_destroy(&semaphore);

    return 0;
}
