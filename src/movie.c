#include "movie.h"

int get_index(const void *movie)
{
    return ((Movie *)movie)->id;
}

Movie create_movie(void)
{
    Movie movie;
    printf("ID:\n");
    scanf("%d", &(movie.id));
    getchar();

    printf("Name:\n");
    fgets(movie.name, BUFF_SZ, stdin);

    printf("Year:\n");
    scanf("%d", &(movie.year));
    getchar();

    printf("URL:\n");
    fgets(movie.name, BUFF_SZ, stdin);

    printf("Director ID:\n");
    scanf("%d", &(movie.director_id));
    getchar();

    return movie;
}