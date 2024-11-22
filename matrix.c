#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DEFAULT_MODE_BINARY 1 // Mode binaire par défaut

// Fonction pour générer une matrice carrée aléatoire
void generate_matrix(int dim, int **matrix) {
    srand(time(NULL));
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            matrix[i][j] = rand() % 100; // Valeurs aléatoires entre 0 et 99
        }
    }
}

// Fonction pour sauvegarder une matrice au format binaire
void save_matrix_binary(const char *filename, int **matrix, int dim) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier binaire");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < dim; i++) {
        fwrite(matrix[i], sizeof(int), dim, file);
    }

    fclose(file);
}

// Fonction pour sauvegarder une matrice au format texte
void save_matrix_text(const char *filename, int **matrix, int dim) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier texte");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            fprintf(file, "%d ", matrix[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

// Fonction pour afficher une matrice binaire
void display_matrix_binary(const char *filename, int dim) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier binaire");
        exit(EXIT_FAILURE);
    }

    int **matrix = malloc(dim * sizeof(int *));
    for (int i = 0; i < dim; i++) {
        matrix[i] = malloc(dim * sizeof(int));
        fread(matrix[i], sizeof(int), dim, file);
    }

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    fclose(file);

    for (int i = 0; i < dim; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Fonction pour afficher une matrice texte
void display_matrix_text(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier texte");
        exit(EXIT_FAILURE);
    }

    int value;
    while (fscanf(file, "%d", &value) != EOF) {
        printf("%d ", value);
        if (fgetc(file) == '\n') {
            printf("\n");
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    int dim = 0;
    char *filename = NULL;
    int mode_binary = DEFAULT_MODE_BINARY;
    int create = 0, display = 0;

    // Parcourir les arguments de la ligne de commande
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            create = 1;
        } else if (strcmp(argv[i], "-a") == 0) {
            display = 1;
        } else if (strcmp(argv[i], "-d") == 0) {
            if (i + 1 < argc) {
                dim = atoi(argv[++i]);
            } else {
                fprintf(stderr, "Erreur : -d nécessite une valeur entière.\n");
                return EXIT_FAILURE;
            }
        } else if (strcmp(argv[i], "-b") == 0) {
            mode_binary = 1;
        } else if (strcmp(argv[i], "-t") == 0) {
            mode_binary = 0;
        } else if (strcmp(argv[i], "-f") == 0) {
            if (i + 1 < argc) {
                filename = argv[++i];
            } else {
                fprintf(stderr, "Erreur : -f nécessite un nom de fichier.\n");
                return EXIT_FAILURE;
            }
        }
    }

    if (!filename || dim <= 0) {
        fprintf(stderr, "Erreur : les arguments -f et -d sont obligatoires.\n");
        return EXIT_FAILURE;
    }

    if (create) {
        // Génération d'une matrice aléatoire
        int **matrix = malloc(dim * sizeof(int *));
        for (int i = 0; i < dim; i++) {
            matrix[i] = malloc(dim * sizeof(int));
        }
        generate_matrix(dim, matrix);

        if (mode_binary) {
            save_matrix_binary(filename, matrix, dim);
        } else {
            save_matrix_text(filename, matrix, dim);
        }

        for (int i = 0; i < dim; i++) {
            free(matrix[i]);
        }
        free(matrix);
    }

    if (display) {
        if (mode_binary) {
            display_matrix_binary(filename, dim);
        } else {
            display_matrix_text(filename);
        }
    }

    return 0;
}
