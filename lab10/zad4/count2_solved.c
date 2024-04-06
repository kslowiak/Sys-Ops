#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

sem_t semaphore1;

int main() {
    int i;
    int j;

    // Inicjalizacja semafora
    sem_init(&semaphore1, 1, 1);

    for (i = 2; i < 200; i = i + 2) {
        sem_wait(&semaphore1);  // Czekaj na dostęp do sekcji krytycznej

        // Początek sekcji krytycznej
        for (j = 0; j < i; j++) {
            printf("%d ", i);
            fflush(stdout);
        }
        sleep(1);
        printf("\n");
        // Koniec sekcji krytycznej

        sem_post(&semaphore1);  // Zwolnij dostęp do sekcji krytycznej
    }

    // Zniszcz semafor
    sem_destroy(&semaphore1);

    return 0;
}
