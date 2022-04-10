#include "headers.h"
#include "pinfo.h"

void pinfo(int pid)
{
    if (pid == -1)
    {
        pid_t p;
        p = getpid();
        pid = p;
    }
    char *path = (char *)malloc(100);
    sprintf(path, "/proc/%d/stat", pid);
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        if (errno == EACCES)
        {
            printf("There is no process with the given pid:%d\n", pid);
            return;
        }
        perror("pinfo error");
        return;
    }
    int a, num_of_spaces = 0;
    char state, buf;
    int pgrp, tgid;
    fscanf(fp, "%d", &a);
    while (num_of_spaces != 2)
    {
        fscanf(fp, "%c", &buf);
        if (buf == ' ')
            num_of_spaces++;
    }
    fscanf(fp, "%c", &state);
    fscanf(fp, "%d", &pgrp);
    fscanf(fp, "%d", &pgrp);
    fscanf(fp, "%d", &tgid);
    fscanf(fp, "%d", &tgid);
    fscanf(fp, "%d", &tgid);
    fclose(fp);
    sprintf(path, "/proc/%d/statm", pid);
    fp = fopen(path, "r");
    if (fp == NULL)
    {
        if (errno == EACCES)
        {
            printf("There is no process with the given pid:%d\n", pid);
            return;
        }
        perror("pinfo error");
        return;
    }
    long unsigned int vm;
    fscanf(fp, "%lu", &vm);
    char *buffer = (char *)malloc(PATH_MAX);
    sprintf(path, "/proc/%d/exe", pid);
    readlink(path, buffer, PATH_MAX);
    printf("pid -- %d\n", pid);
    if (pgrp != tgid)
        printf("Process Status -- %c\n", state);
    else
        printf("Process Status -- %c+\n", state);
    printf("memory -- %lu\n", vm);
    printf("Executable Path -- %s\n", buffer);
    free(path);
    free(buffer);
}