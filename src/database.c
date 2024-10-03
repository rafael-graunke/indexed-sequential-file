#include <stdlib.h>
#include "database.h"

void *read_bin_file(char *filename, size_t __size, int *count)
{
    FILE *source = fopen(filename, "rb");
    fseek(source, 0, SEEK_END);
    *count = ftell(source) / __size;
    rewind(source);

    void *records = malloc(__size * (*count));
    fread(records, __size, *count, source);
    fclose(source);

    return records;
}

int compare_index(const void *index1, const void *index2)
{
    return ((NumericIndex *)index1)->index - ((NumericIndex *)index2)->index;
}

NumericIndex *generate_index(char *input, char *output, size_t __size, __getindex_fn_t __index)
{
    int count;
    void *records = read_bin_file(input, __size, &count);
    NumericIndex *indexes = (NumericIndex *)malloc(sizeof(NumericIndex) * count);
    for (int i = 0; i < count; i++)
    {
        indexes[i].index = __index((records + (__size * i)));
        indexes[i].address = i * __size;
        indexes[i].next = -1;
    }

    qsort(indexes, count, sizeof(NumericIndex), compare_index);

    return indexes;
}

long binsearch_in_file(FILE *source, size_t __size, int start, int end, void *match, int *pos, __compar_fn_t __compare)
{
    int middle;
    void *cursor = malloc(__size);

    if (start <= end)
    {
        middle = (end + start) / 2;
        fseek(source, __size * middle, SEEK_SET);
        fread(cursor, __size, 1, source);

        printf("%d\t%d\t%d\n", start, middle, end);

        int cmp = __compare(match, cursor);
        if (cmp == 0)
        {
            *pos = middle;
            return (*pos) * __size;
        }

        else if (cmp < 0)
            return binsearch_in_file(source, __size, start, middle - 1, match, pos, __compare);
        else
            return binsearch_in_file(source, __size, middle + 1, end, match, pos, __compare);
    }

    return -1;
}
