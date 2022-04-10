#include "headers.h"
#include "utilities.h"

void remove_space(char *input, char *output)
{
    // char text[100], blank[100];
    int c = 0, d = 0;
    while (input[c] != '\0')
    {
        if (input[c] == '\t')
            input[c] = ' ';
        c++;
    }
    c = 0;
    while (input[c] != '\0')
    {
        if (c == 0 && input[c] == ' ')
        {
            c++;
            continue;
        }
        if (input[c] == ' ' && input[c + 1] == '\0')
        {
            break;
        }
        if (!(input[c] == ' ' && input[c + 1] == ' '))
        {
            output[d] = input[c];
            d++;
        }
        c++;
    }
    output[d] = '\0';
}

