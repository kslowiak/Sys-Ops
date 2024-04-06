#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        // Nie podano żadnych argumentów, wyświetl wszystkie zmienne środowiskowe
        extern char **environ;
        char **env = environ;
        
        while (*env != NULL) {
            printf("%s\n", *env);
            env++;
        }
    } else {
        // Wyświetl wartości zmiennych środowiskowych zgodnie z argumentami
        int i;
        for (i = 1; i < argc; i++) {
            char *value = getenv(argv[i]);
            if (value != NULL) {
                printf("%s=%s\n", argv[i], value);
            } else {
                printf("Zmienna %s nie istnieje\n", argv[i]);
            }
        }
    }

    return 0;
}