#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <stdio.h>
#include <stdbool.h>

typedef int (*__getindex_fn_t)(const void *);

typedef struct s_num_index
{
    long index;
    long address;
    long next; // on extension file
} NumericIndex;

void *read_bin_file(char *filename, size_t __size, int *count);
NumericIndex *fetch_indexes(char *source, char *target, size_t __size, __getindex_fn_t get_index, int *index_count);
NumericIndex *generate_index(char *input, char *output, size_t __size, __getindex_fn_t __index);
long append_to_file(char *filename, void *value, size_t __size);
long append_to_extension(NumericIndex index);
long search_extension(int id, long initial_addr, long *curr_addr);

long binsearch_in_file(FILE *source, size_t __size, int start, int end, void *match, int *pos, __compar_fn_t __compare);
NumericIndex binsearch_index(NumericIndex *indexes, int start, int end, int id, int *pos, bool *found);
int count_records(char *filename, size_t __size);

#endif
