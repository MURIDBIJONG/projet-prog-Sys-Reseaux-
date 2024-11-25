#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 10  // Valeur maximale à atteindre (+N, -N)

// Variables partagées
int counter = 0;
int values[2 * N];  // Stocker les valeurs du compteur
int index = 0;

// Sémaphores
sem_t sem_thread1;
sem_t sem_thread2;

// Fonction pour le thread 1
void* thread1_task(void* arg) {
    for (int i = 1; i <= N; i++) {
        sem_wait(&sem_thread1);  // Attendre que ce soit le tour de thread 1
        counter++;
        values[index++] = counter;
        printf("Thread 1 : +1, compteur = %d\n", counter);
        sem_post(&sem_thread2);  // Donner la main à thread 2
    }
    pthread_exit(NULL);
}

// Fonction pour le thread 2
void* thread2_task(void* arg) {
    for (int i = 1; i <= N; i++) {
        sem_wait(&sem_thread2);  // Attendre que ce soit le tour de thread 2
        counter--;
        values[index++] = counter;
        printf("Thread 2 : -1, compteur = %d\n", counter);
        sem_post(&sem_thread1);  // Donner la main à thread 1
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t t1, t2;

    // Initialiser les sémaphores
    sem_init(&sem_thread1, 0, 1);  // Thread 1 démarre
    sem_init(&sem_thread2, 0, 0);  // Thread 2 attend

    // Créer les threads
    pthread_create(&t1, NULL, thread1_task, NULL);
    pthread_create(&t2, NULL, thread2_task, NULL);

    // Attendre que les threads se terminent
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Libérer les sémaphores
    sem_destroy(&sem_thread1);
    sem_destroy(&sem_thread2);

    // Afficher les résultats sous forme de liste
    printf("\nÉvolution du compteur :\n");
    for (int i = 0; i < 2 * N; i++) {
        printf("%d ", values[i]);
    }
    printf("\n");

    return 0;
}
