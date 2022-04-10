#include "cd.h"
#include "headers.h"

extern char HOME[PATH_MAX];
extern int errno;
extern char prev_path[PATH_MAX];

int cd(int argc, char *argv[])
{
    char temp_prev_path[PATH_MAX];
    getcwd(temp_prev_path, PATH_MAX);
    static char IS_CALLED_CD = 0;
    int x;
    if (argc > 2)
    {
        RED
            printf("cd:");
        WHITE
        printf("Too many arguments\n");
        return 0;
    }
    if (argc == 1 || strcmp(argv[1], "~") == 0)
    {

        x = chdir(HOME);
    }
    else if (argv[1][0] == '-')
    {
        if (IS_CALLED_CD != 0)
        {
            chdir(prev_path);
        }
    }
    else
    {

        x = chdir(argv[1]);
    }
    if (x == -1)
    {
        perror("\033[0;31mcd\033[0;37m");
    }
    strcpy(prev_path, temp_prev_path);
    IS_CALLED_CD = 1;
}
