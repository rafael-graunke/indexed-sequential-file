#ifndef _MOVIE_H_
#define _MOVIE_H_

#include <stdio.h>

#define BUFF_SZ 256

typedef struct s_movie {
    int id;
    int year;
    int director_id;
    char name[BUFF_SZ];
    char url[BUFF_SZ];
} Movie;

int get_index(const void *movie);
Movie create_movie(void);

#endif