#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <stdio.h>

typedef int (*__getindex_fn_t)(const void *);

typedef struct s_num_index
{
    long index;
    long address;
    long next; // on extension file
} NumericIndex;

void *read_bin_file(char *filename, size_t __size, int *count);
NumericIndex *generate_index(char *input, char *output, size_t __size, __getindex_fn_t __index);

long binsearch_in_file(FILE *source, size_t __size, int start, int end, void *match, int *pos, __compar_fn_t __compare);

#endif
