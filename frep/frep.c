#include <stdio.h>
#include <stdlib.h>

#define l % count

char *total_buffer;

size_t count;

struct
{
    unsigned reps;
    unsigned contiguous : 1;         // will add functionality later
    unsigned start_at_beginning : 1; // will add functionality later
} data;

typedef struct
{
    char *start;
    char *end;
    int width;
} rep;

rep master_rep;
rep _working_rep;

void find_rep()
{
    rep working_rep = {.start = total_buffer, .end = total_buffer, .width = 1};
    _Bool parent_itt = 1;
    int index = 0;
    int i;
    while (index < count)
    {
    expand:
        for (i = 0; i < working_rep.width; i++)
        {
            if (working_rep.start[i l] != working_rep.end[(i + 1 l)])
            {
                if (parent_itt && working_rep.width < count / 2)
                {
                    working_rep.end++;
                    working_rep.width++;
                    goto expand;
                }
                goto break_out;
            }
        }

        data.reps++;

        master_rep.end++;
        master_rep.width++;

        working_rep.start = working_rep.end + 1;
        working_rep.end = working_rep.start + working_rep.width - 1;
        parent_itt = 0;

        index += working_rep.width;
    }
break_out:

    if (data.reps == 0)
    {
        master_rep.width = 0; // don't could the default width of 1 char if there was no repetition in the stream
        data.reps = -1;
    }
}

int main()
{
    size_t buffer_size = 1024; // Initial buffer size
    count = 0;
    total_buffer = (char *)malloc(buffer_size);
    if (total_buffer == NULL)
    {
        perror("malloc failed");
        return EXIT_FAILURE;
    }

    int ch;
    while ((ch = fgetc(stdin)) != EOF)
    {
        // Expand the buffer if necessary
        if (count >= buffer_size)
        {
            buffer_size *= 2;
            char *new_buffer = (char *)realloc(total_buffer, buffer_size);
            if (new_buffer == NULL)
            {
                perror("realloc failed");
                free(total_buffer);
                return EXIT_FAILURE;
            }
            total_buffer = new_buffer;
        }

        // Add the character to the buffer
        total_buffer[count++] = (char)ch;
    }

    // Null-terminate the buffer
    total_buffer[count] = 0;

    master_rep.start = master_rep.end = total_buffer;
    master_rep.width = 1;

    find_rep();

    master_rep.end[1] = 0;

    fprintf(stderr, "\n\n%s\n\nRepetition %sfound\nsize: %u\ncount: %u\n", master_rep.start, data.reps <= 0 ? "not " : "", master_rep.width, data.reps + 1);

    // Free the buffer
    free(total_buffer);
    return 0;
}
