#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void join_files(const char *output_filename, int num_parts, char *parts[]) {
    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file) {
        perror("Erreur lors de l'ouverture du fichier de destination");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    for (int i = 0; i < num_parts; i++) {
        FILE *part_file = fopen(parts[i], "rb");
        if (!part_file) {
            perror("Erreur lors de l'ouverture d'un fichier de partie");
            fclose(output_file);
            exit(EXIT_FAILURE);
        }

        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), part_file)) > 0) {
            fwrite(buffer, 1, bytes_read, output_file);
        }

        fclose(part_file);
    }

    fclose(output_file);
    printf("Fichiers réunis avec succès dans '%s'.\n", output_filename);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage : %s nomfichier sortie part1 part2 ...\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *output_filename = argv[1];
    join_files(output_filename, argc - 2, &argv[2]);
    return EXIT_SUCCESS;
}
