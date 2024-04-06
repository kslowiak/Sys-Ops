// producent.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

#define SHM_SIZE 1024

struct SharedMemory {
    int data;
    char text[256];
};

void wait_semaphore(int sem_id, int sem_num) {
    struct sembuf op = {sem_num, -1, 0};
    semop(sem_id, &op, 1);
}

void signal_semaphore(int sem_id, int sem_num) {
    struct sembuf op = {sem_num, 1, 0};
    semop(sem_id, &op, 1);
}

int main() {
    key_t key = ftok("shared_memory_key", 65);
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    int sem_id = semget(key, 3, IPC_CREAT | 0666);  // Dodatkowy semafor do zabezpieczenia wyświetlania

    struct SharedMemory *shared_memory = (struct SharedMemory *)shmat(shmid, NULL, 0);

    // Inicjalizacja semaforów
    semctl(sem_id, 0, SETVAL, 1);  // Producent - dostęp do pamięci współdzielonej
    semctl(sem_id, 1, SETVAL, 0);  // Konsument - oczekiwanie na dane
    semctl(sem_id, 2, SETVAL, 0);  // Dla zabezpieczenia wyświetlania

    while (1) {
        // Oczekuj na zezwolenie do zapisu
        wait_semaphore(sem_id, 0);

        // Wprowadź dane do pamięci współdzielonej
        printf("Producent: Podaj liczbe calkowita: ");
        scanf("%d", &(shared_memory->data));
        printf("Producent: Podaj tekst: ");
        scanf("%s", shared_memory->text);

        // Zapisz dane do pamięci współdzielonej
        printf("Producent: Zapisano dane.\n");
        signal_semaphore(sem_id, 1);
    }

    shmdt(shared_memory);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);
    return 0;
}
