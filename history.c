#include "history.h"
#include "headers.h"
void __write_history(int num);
void init_history()
{
    num_history = 0;
    FILE *fp = fopen("history.txt", "r");
    if (fp == NULL)
    {
        fopen("history.txt", "w");
        fclose(fp);
        return;
    }
    fprintf(fp, "\n");
    for (int i = 0; i < 20; i++)
    {
        int j = 0;
        char c;
        c = fgetc(fp);
        while (c != '\n' && c != EOF)
        {
            history_array[i][j++] = c;
            c = fgetc(fp);
        }
        if (c == EOF)
            break;
        history_array[i][j] = '\0';
        num_history++;
    }
    fclose(fp);
}
void add_history(char *inp)
{
    if (num_history > 0)
    {
        if (strcmp(history_array[num_history - 1], inp) == 0 )
            return;
    }
    if (num_history < 20)
        strcpy(history_array[num_history++], inp);
    else
    {
        for (int i = 0; i < 20; i++)
        {
            strcpy(history_array[i], history_array[i + 1]);
        }
        strcpy(history_array[19], inp);
    }
}
void write_back()
{
    FILE *fp = fopen("history.txt", "w");
    for (int i = 0; i < num_history; i++)
    {
        fprintf(fp, "%s\n", history_array[i]);
    }
    fclose(fp);
}
void write_history(int agrc, char *argv[])
{
    if (agrc == 1)
        __write_history(num_history);
    else if (agrc == 2)
    {
        int x = atoi(argv[1]);
        __write_history(x);
    }
    else
        printf("Error in history command");
}
void __write_history(int num)
{
    if (num < num_history)
    {
        for (int i = num_history - num; i < num_history; i++)
            printf("%s\n", history_array[i]);
    }
    else
    {
        for (int i = 0; i < num_history; i++)
            printf("%s\n", history_array[i]);
    }
}