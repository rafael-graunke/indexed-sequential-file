#include <stdio.h>
#include <stdlib.h>

#include "movie.h"
#include "database.h"

int get_index(const void *arg)
{
    return ((Movie *) arg)->id;
}

int compare_movie_id(const void *id, const void *movie)
{
    return *((int *)id) - ((Movie *) movie)->id;
}

int main(void) {
    int count;
    Movie *movies = (Movie *) read_bin_file("input/arqMovies.bin", sizeof(Movie), &count);

    NumericIndex *indexes = generate_index("input/arqMovies.bin", "input/index.bin", sizeof(Movie), get_index);

    // FILE *file = fopen("input/arqMovies.bin", "rb");
    // fseek(file, indexes[99].address, SEEK_SET);
    // Movie movie;
    // fread(&movie, sizeof(Movie), 1, file);
    // printf("%s\n", movie.name);

    FILE *file = fopen("input/arqMovies.bin", "rb");
    fseek(file, 0, SEEK_END);
    int last = ftell(file) / sizeof(Movie) - 1;
    rewind(file);
    int match = 100;
    int pos;
    printf("Caiu aq\n");
    long address = binsearch_in_file(file, sizeof(Movie), 0, last, &match, &pos, compare_movie_id);
    printf("%ld\n", address);

    return EXIT_SUCCESS;
}
