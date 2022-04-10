#include "prompt.h"
#include "headers.h"

extern char HOME[PATH_MAX];

void prompt()
{
    struct utsname unameData;
    uname(&unameData);                            // Get system information
    char *user = getenv("USER");                  // Get user name
    printf("\033[0;32m%s", user);                 // Print user name
    printf("\033[0;32m@%s:", unameData.nodename); // Print host name

    //Print current working directory
    char cwd[PATH_MAX], *temp_dir, *temp_basename[PATH_MAX], temp_cwd[PATH_MAX], temp_cwd2[PATH_MAX];
    //we have to use tempcwd as basename and dirname change the cwd argument

    //getting cwd
    getcwd(cwd, sizeof(cwd));
    int j = 0;

    //finding wether to print the full path or ~
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        if (strcmp(cwd, HOME) == 0)
        {
            PURPLE
            printf("~>"); // If we are at home directory, print ~>
        }
        else
        {
            while (strcmp(cwd, "/") != 0)
            {
                strcpy(temp_cwd, cwd);
                strcpy(temp_cwd2, cwd);
                temp_dir = dirname(temp_cwd);             //getting the parent directory
                temp_basename[++j] = basename(temp_cwd2); //getting the basename
                if (strcmp(temp_dir, HOME) == 0)
                {
                    PURPLE
                    printf("~");

                    for (; j > 0; j--)
                    {
                        PURPLE
                        printf("/%s", temp_basename[j]);
                    }
                    PURPLE
                    printf(">");
                    goto EXIT;
                }
                strcpy(cwd, temp_dir);
            }
            getcwd(cwd, sizeof(cwd));
            PURPLE
            printf("%s>", cwd);
        }
    }
    else
    {
        perror("getcwd() error");
    }
EXIT:
    WHITE
}
