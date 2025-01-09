#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define START_PORT 0
#define END_PORT 3000
#define TARGET_IP "127.0.0.1"

int main() {
    int sock;
    struct sockaddr_in target;
    int port;

    // Configuration de l'adresse cible
    memset(&target, 0, sizeof(target));
    target.sin_family = AF_INET;
    target.sin_addr.s_addr = inet_addr(TARGET_IP);

    printf("Scan des ports TCP ouverts sur %s de %d à %d...\n", TARGET_IP, START_PORT, END_PORT);

    for (port = START_PORT; port <= END_PORT; port++) {
        // Création du socket
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("Erreur lors de la création du socket");
            return EXIT_FAILURE;
        }

        target.sin_port = htons(port); // Port cible

        // Tentative de connexion
        if (connect(sock, (struct sockaddr*)&target, sizeof(target)) == 0) {
            printf("Port ouvert : %d\n", port); // Affichage du port ouvert
        }

        close(sock); // Fermeture du socket
    }

    printf("Scan terminé.\n");
    return 0;
}
