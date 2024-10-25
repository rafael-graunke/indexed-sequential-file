#include <stdlib.h>
#include "database.h"

int count_records(char *filename, size_t __size)
{
    FILE *source = fopen(filename, "rb");
    fseek(source, 0, SEEK_END);
    int count = ftell(source) / __size;
    fclose(source);
    return count;
}

void *read_bin_file(char *filename, size_t __size, int *count)
{
    FILE *source = fopen(filename, "rb");
    *count = count_records(filename, __size);
    void *records = malloc(__size * (*count));
    fread(records, __size, *count, source);
    fclose(source);

    return records;
}

int compare_index(const void *index1, const void *index2)
{
    return ((NumericIndex *)index1)->index - ((NumericIndex *)index2)->index;
}

NumericIndex *fetch_indexes(char *source, char *target, size_t __size, __getindex_fn_t get_index, int *index_count)
{
    FILE *index_file = fopen(target, "rb");
    NumericIndex *indexes;
    // If file doesn't exist
    if (index_file == NULL) // Then generate new indexes
    {
        indexes = generate_index(source, target, __size, get_index);
    }

    // Else read from index file
    else
    {
        printf("Reading indexes...\n");
        indexes = (NumericIndex *)read_bin_file(target, sizeof(NumericIndex), index_count);
    }

    return indexes;
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
    FILE *out = fopen(output, "wb");
    fwrite(indexes, sizeof(NumericIndex), count, out);

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

NumericIndex binsearch_index(NumericIndex *indexes, int start, int end, int id, int *pos, bool *found)
{
    int middle;
    *found = false;
    NumericIndex curr;
    if (start <= end)
    {
        middle = (end + start) / 2;
        *pos = middle;
        curr = indexes[middle];
        int cmp = id - curr.index;
        if (cmp == 0)
        {
            *found = true;
            return curr;
        }

        else if (cmp < 0)
            return binsearch_index(indexes, start, middle - 1, id, pos, found);
        else
            return binsearch_index(indexes, middle + 1, end, id, pos, found);
    }

    return curr;
}

long append_to_file(char *filename, void *value, size_t __size)
{
    FILE *file = fopen(filename, "ab");
    fwrite(value, __size, 1, file);
    fseek(file, 0, SEEK_END);
    long address = ftell(file) - __size;
    fclose(file);

    return address;
}

long append_to_extension(NumericIndex index)
{
    return append_to_file("input/extension.bin", &index, sizeof(NumericIndex));
}

long search_extension(int id, long initial_addr, long *curr_addr)
{
    NumericIndex index;
    FILE *extension = fopen("input/extension.bin", "rb");
    fseek(extension, initial_addr, SEEK_SET);
    fread(&index, sizeof(NumericIndex), 1, extension);
    *curr_addr = ftell(extension) - sizeof(NumericIndex);
    fclose(extension);

    if (index.next == -1)
    {
        if (index.index == id)
            return index.address;
        return -1;
    }

    return search_extension(id, index.next, curr_addr);
}