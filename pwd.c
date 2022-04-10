#include "headers.h"
#include "pwd.h"

void pwd()
{
    char buffer[PATH_MAX];
    getcwd(buffer, PATH_MAX);
    printf("%s\n", buffer);
}