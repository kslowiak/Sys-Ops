#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2;
    int status;

    pid1 = fork();

    if (pid1 == 0) {
        execl("./nowy1", "nowy1", NULL);
        perror("Blad execl dla nowy1");
        exit(1);
    } else if (pid1 < 0) {
        perror("Blad fork dla nowy1");
    } else {
        pid2 = fork();

        if (pid2 == 0) {
            execl("./nowy2", "nowy2", NULL);
            perror("Blad execl dla nowy2");
            exit(1);
        } else if (pid2 < 0) {
            perror("Blad fork dla nowy2");
        } else {
        // Proces macierzysty
            waitpid(pid1, &status, 0);
            printf("Proces potomny PID: %d zakończył się z kodem: %d sygnał: %d\n", pid1, WEXITSTATUS(status), WTERMSIG(status));
            waitpid(pid2, &status, 0);
	        printf("Proces potomny PID: %d zakończył się z kodem: %d sygnał: %d\n", pid2, WEXITSTATUS(status), WTERMSIG(status));
            }
     }
        
     return 0;
}
