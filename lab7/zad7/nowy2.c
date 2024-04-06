#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    printf("Proces nowy2 otrzymał sygnał %d\n", sig);
    exit(1);
}

int main() {
    signal(SIGINT, handler);
    signal(SIGTERM, handler);

    printf("Proces nowy2 rozpoczął działanie.\n");
    fflush(stdout);

    sleep(30);

    printf("Proces nowy2 zakończył działanie.\n");
    return 0;
}
