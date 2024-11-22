#include <stdio.h>
#include <stdlib.h>

void split_file(const char *filename, size_t chunk_size) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier source");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    int part_number = 1;
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        char part_filename[256];
        snprintf(part_filename, sizeof(part_filename), "%s.part%d", filename, part_number);

        FILE *part_file = fopen(part_filename, "wb");
        if (!part_file) {
            perror("Erreur lors de l'ouverture du fichier de destination");
            fclose(file);
            exit(EXIT_FAILURE);
        }

        size_t bytes_to_write = chunk_size < bytes_read ? chunk_size : bytes_read;
        fwrite(buffer, 1, bytes_to_write, part_file);
        fclose(part_file);

        chunk_size -= bytes_to_write;
        if (chunk_size == 0) {
            chunk_size = bytes_read; // Nouvelle taille pour le prochain fichier
            part_number++;
        }
    }

    fclose(file);
    printf("Fichier divisé avec succès en %d parties.\n", part_number);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage : %s nomfichier taille\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    size_t chunk_size = atoi(argv[2]);
    if (chunk_size <= 0) {
        fprintf(stderr, "La taille doit être un entier positif.\n");
        return EXIT_FAILURE;
    }

    split_file(filename, chunk_size);
    return EXIT_SUCCESS;
}
