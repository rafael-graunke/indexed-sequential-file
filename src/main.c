#include <stdio.h>
#include <stdlib.h>

#include "movie.h"
#include "database.h"

long indexed_binary_search(void)
{
    int index_count;

    NumericIndex *indexes = fetch_indexes("input/arqMovies.bin", "input/index.bin", sizeof(Movie), get_index, &index_count);

    // Get user input
    int id;
    printf("What ID to search for?\n");
    scanf("%d", &id);

    // Search for address
    bool found;
    int pos;
    NumericIndex index = binsearch_index(indexes, 0, index_count, id, &pos, &found);
    long address = index.address;

    // If not found at base file
    if (!found)
    {
        printf("Not found.\n");
        return -1;
        // FILE *extension = fopen("input/extension.bin", "rb");
        // if (extension == NULL)
        // {
        //     printf("Movie with ID %d not found!\n", id);
        //     return -1;
        // }

        // // Then search extension
        // long next_address;
        // index = indexes[pos];
        // while ((next_address = index.next) != -1)
        // {
        //     printf("%ld\n", next_address);
        //     fseek(extension, next_address, SEEK_SET);
        //     fread(&index, sizeof(NumericIndex), 1, extension);
        // }
        // fread(&index, sizeof(NumericIndex), 1, extension);

        // if (index.index != id)
        // {
        //     printf("Movie with ID %d not found!\n", id);
        //     return -1;
        // }

        // address = index.address;
    }

    // Read movie at the found address
    Movie movie;
    FILE *source = fopen("input/arqMovies.bin", "rb");
    fseek(source, address, SEEK_SET);
    fread(&movie, sizeof(Movie), 1, source);
    fclose(source);

    printf("Found movie \"%s\"!\n", movie.name);

    return address;
}

void add_new_movie(void)
{
    // Create new movie
    Movie movie = create_movie();

    long movie_address = append_to_file("input/arqMovies.bin", &movie, sizeof(Movie));

    // Create new index for the new movie
    NumericIndex new_index = {};
    new_index.index = movie.id;
    new_index.address = movie_address;
    new_index.next = -1;

    long new_index_address = append_to_extension(new_index);
    printf("New index address is: %ld\n", new_index_address);

    // Find last index position where new index should be inserted
    int index_count;
    NumericIndex *indexes = fetch_indexes("input/arqMovies.bin", "input/index.bin", sizeof(Movie), get_index, &index_count);
    int pos;
    bool found;
    binsearch_index(indexes, 0, index_count, movie.id, &pos, &found);
    
    // Find index to "extend"
    NumericIndex index = indexes[pos];
    FILE *index_file = fopen("input/index.bin", "rwb");

    if (index.next != -1)
    {
        // Then index already has linked list
        NumericIndex extension_index;
        FILE *extension_file = fopen("input/extension.bin", "rb");
        fseek(extension_file, index.next, SEEK_SET);
        fread(&extension_index, sizeof(NumericIndex), 1, extension_file);

        long curr_address = 0;
        while(extension_index.next != -1)
        {
            curr_address = extension_index.next;
            fseek(extension_file, extension_index.next, SEEK_SET);
            fread(&extension_index, sizeof(NumericIndex), 1, extension_file);
        }
        fclose(extension_file);
        extension_file = fopen("input/extension.bin", "wb");

        extension_index.next = new_index_address;
        fseek(extension_file, curr_address, SEEK_SET);
        fwrite(&index, sizeof(NumericIndex), 1, extension_file);
        fclose(extension_file);

        return;
    }

    index.next = new_index_address;
    fseek(index_file, pos * sizeof(NumericIndex), SEEK_SET);
    fwrite(&index, sizeof(NumericIndex), 1, index_file);
    fclose(index_file);
}

int menu(void)
{
    int input = -1;
    while (input != 0)
    {
        printf("1 - Indexed binary search;\n");
        printf("2 - Add new movie;\n");
        printf("3 - Persist extension file;\n");
        printf("0 - Quit.\n");

        scanf("%d", &input);
        switch (input)
        {
        case 1:
            indexed_binary_search();
            break;
        case 2:
            add_new_movie();
            break;
        case 3:
            break;
        case 0:
            break;
        default:
            printf("Invalid option.\n");
            break;
        }
    }

    return EXIT_SUCCESS;
}

int main(void)
{
    return menu();
}
