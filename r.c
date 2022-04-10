#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
    char *a[5];
    a[1] = (char*)malloc(12);
    a[0] = (char*)malloc(12);
    strcpy(a[0], "ls");
    strcpy(a[1], "-a");
    execvp(a[0], a);
}