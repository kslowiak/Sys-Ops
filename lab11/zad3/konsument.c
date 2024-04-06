// konsument.c
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
    int shmid = shmget(key, SHM_SIZE, 0666);
    int sem_id = semget(key, 3, 0666);  // Dodatkowy semafor do zabezpieczenia wyświetlania

    struct SharedMemory *shared_memory = (struct SharedMemory *)shmat(shmid, NULL, 0);

    while (1) {
        // Oczekuj na dane
        wait_semaphore(sem_id, 1);

        // Wyświetlaj dane tylko raz
        wait_semaphore(sem_id, 2);

        // Odczytaj dane z pamięci współdzielonej
        printf("Konsument: Odczytano dane - Liczba: %d, Tekst: %s\n", shared_memory->data, shared_memory->text);

        // Zezwól na zapis nowych danych
        signal_semaphore(sem_id, 0);

        // Zablokuj możliwość ponownego wyświetlania
        signal_semaphore(sem_id, 2);
    }

    shmdt(shared_memory);
    return 0;
}
