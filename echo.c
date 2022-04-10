#include "headers.h"
#include "echo.h"

void echo(char *string)
{
    remove_space(string, string);
    string[0] = ' ';
    string[1] = ' ';
    string[2] = ' ';
    string[3] = ' ';
    remove_space(string, string);
    remove_space(string, string);
    printf("%s\n", string);
}