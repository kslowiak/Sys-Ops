#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#define PORT 8080
#define MAX_PENDING 10
#define MAX_LINE 1024

void send_html(int client_sock) {
    FILE *file = fopen("site.html", "r");
    if (file == NULL) {
        printf("Could not open file");
        return;
    }

    char response[MAX_LINE];
    char http_header[] = "HTTP/1.0 200 OK\nContent-Type: text/html\n\n";
    send(client_sock, http_header, strlen(http_header), 0);

    while (fgets(response, MAX_LINE, file) != NULL) {
        send(client_sock, response, strlen(response), 0);
    }

    fclose(file);
}

int main() {
    int server_sock, client_sock; // Deklaracja zmiennych przechowujących deskryptory gniazd serwera i klienta
    struct sockaddr_in server_addr, client_addr; // Struktury przechowujące informacje o adresach serwera i klienta
    socklen_t sin_len = sizeof(client_addr); // Zmienna przechowująca długość struktury klienta
    char buf[MAX_LINE]; // Bufor przechowujący dane odbierane od klienta

    // Tworzenie gniazda serwera
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket");
        exit(1);
    }

    // Inicjalizacja struktury server_addr ustawiająca parametry serwera
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Przypisanie gniazda do określonego adresu i portu
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_sock);
        exit(1);
    }

    // Ustawienie gniazda w trybie nasłuchiwania na połączenia
    if (listen(server_sock, MAX_PENDING) == -1) {
        perror("listen");
        close(server_sock);
        exit(1);
    }

    // Komunikat informujący o uruchomieniu serwera
    printf("Server started on port %d\n", PORT);

    // Główna pętla obsługująca połączenia od klientów
    while (1) {
        // Akceptowanie połączenia od klienta
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &sin_len);

        // Obsługa błędu w przypadku nieudanego połączenia
        if (client_sock == -1) {
            perror("Can't accept");
            continue;
        }

        // Komunikat informujący o połączeniu z klientem
        printf("Client connected\n");

        // Zerowanie bufora
        memset(buf, 0, MAX_LINE);

        // Odbieranie danych od klienta
        recv(client_sock, buf, sizeof(buf), 0);

        // Wysyłanie zawartości pliku HTML do klienta
        send_html(client_sock);

        // Zamykanie połączenia z klientem
        close(client_sock);
    }

    return 0;
}


