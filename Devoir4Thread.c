#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_STEPS 100  // Taille maximale de n

// Variables globales
int n;                 // Limite des incréments et décréments
int current_value = 0; // Valeur actuelle (entre -n et +n)
sem_t sem_thread1, sem_thread2; // Sémaphores pour synchronisation
FILE* output_file;     // Fichier pour enregistrer les résultats

// Fonction exécutée par le Thread 1
void* thread1_task(void* arg) {
    for (int i = 1; i <= n; i++) {
        sem_wait(&sem_thread1); // Attendre le feu vert pour le thread 1
        current_value = i;
        fprintf(output_file, "%d\n", current_value); // Enregistrer la valeur
        printf("Thread 1 : %d\n", current_value);
        sem_post(&sem_thread2); // Libérer le thread 2
    }
    pthread_exit(NULL);
}

// Fonction exécutée par le Thread 2
void* thread2_task(void* arg) {
    for (int i = n; i >= -n; i--) {
        sem_wait(&sem_thread2); // Attendre le feu vert pour le thread 2
        current_value = i;
        fprintf(output_file, "%d\n", current_value); // Enregistrer la valeur
        printf("Thread 2 : %d\n", current_value);
        if (i > -n) {
            sem_post(&sem_thread1); // Libérer le thread 1 pour décrémenter
        }
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t thread1, thread2;

    // Demander la limite n
    printf("Entrez la valeur de n (limite des incréments/décréments) : ");
    scanf("%d", &n);

    if (n > MAX_STEPS) {
        fprintf(stderr, "Erreur : n doit être inférieur ou égal à %d.\n", MAX_STEPS);
        return EXIT_FAILURE;
    }

    // Ouvrir le fichier pour enregistrer les résultats
    output_file = fopen("resultats.txt", "w");
    if (!output_file) {
        perror("Erreur lors de l'ouverture du fichier");
        return EXIT_FAILURE;
    }

    // Initialiser les sémaphores
    sem_init(&sem_thread1, 0, 1); // Thread 1 démarre
    sem_init(&sem_thread2, 0, 0); // Thread 2 attend

    // Créer les threads
    pthread_create(&thread1, NULL, thread1_task, NULL);
    pthread_create(&thread2, NULL, thread2_task, NULL);

    // Attendre la fin des threads
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Nettoyer les sémaphores et fermer le fichier
    sem_destroy(&sem_thread1);
    sem_destroy(&sem_thread2);
    fclose(output_file);

    printf("Les résultats sont enregistrés dans 'resultats.txt'.\n");
    printf("Utilisez un outil de visualisation pour tracer la courbe.\n");

    return EXIT_SUCCESS;
}
