#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    printf("Proces nowy1 otrzymał sygnał %d\n", sig);
    exit(1);
}

int main() {
    signal(SIGINT, handler);
    signal(SIGTERM, handler);

    printf("Proces nowy1 rozpoczął działanie.\n");
    fflush(stdout);

    sleep(20);

    printf("Proces nowy1 zakończył działanie.\n");
    return 0;
}
