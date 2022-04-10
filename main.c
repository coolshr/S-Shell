#include "prompt.h"
#include "headers.h"
void promptandinput();
char HOME[PATH_MAX];
extern int errno;
void die(const char *s)
{
    perror(s);
    exit(1);
}
struct bgprc
{
    pid_t pid;
    char name[100];
} bgProc[100];
// pid_t bgProc[100];
int bgProcNum = 0;
struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}
void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}
void ex(char **inp)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("Fork");
        return;
    }
    else if (pid == 0)
    {
        if (execvp(inp[0], inp) < 0)
        {
            perror("Failed to run command");
        }
        exit(0);
    }
    else
    {
        // waiting for child to terminate
        wait(NULL);
        return;
    }
}
void handle_sigquit(int sig)
{
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    if (pid > 0)
    {
        for (int i = 0; i < 100; i++)
            if (pid == bgProc[i].pid)
            {

                WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS ? printf("\n%s with pid %d exited normally \n", &bgProc[i].name[1], pid) : printf("\n%s with pid %d exited abnormally\n", &bgProc[i].name[1], pid);
                // printf("\n%s with pid %d exited \n", &name[1], pid);
                bgProc[i].pid = 0;
                bgProc[i].name[0] = '\0';
                // fclose(fp);
                // bgProcNum--;
                break;
            }
        promptandinput();
    }
}
void runBg(int argc, char *argv[])
{
    pid_t p = fork();
    if (p == -1)
    {
        perror("Fork:");
        return;
    }
    else if (p == 0) //Child
    {
        if (execvp(argv[0], argv) < 0)
        {
            perror("execvp:");
        }
    }
    else //parent
    {
        printf("%d\n", p);
        bgProc[bgProcNum % 100].pid = p;
        char path[PATH_MAX];
        sprintf(path, "/proc/%d/stat", p);
        FILE *fp = fopen(path, "r");
        int temp;
        fscanf(fp, "%d", &temp);
        char name[100];
        fscanf(fp, "%s", name);
        // name[0] = ' ';
        name[strlen(name) - 1] = '\0';
        strcpy(bgProc[bgProcNum++ % 100].name, name);
        fclose(fp);
        return;
    }
}
int parse(char *inp)
{
    char *token = strtok(inp, ";");
    while (token != NULL)
    {
        size_t temp = PATH_MAX;
        size_t char_read = 0;
        wordexp_t p;
        char *token_temp = (char *)malloc(strlen(token));
        strcpy(token_temp, token);
        if (strchr(token_temp, '&') != NULL)
        {
            strchr(token_temp, '&')[0] = '\0';
            wordexp(token_temp, &p, 0);
            runBg(p.we_wordc, p.we_wordv);
        }
        else
        {
            wordexp(token, &p, 0);
            if (strcmp(p.we_wordv[0], "cd") == 0)
            {
                cd(p.we_wordc, p.we_wordv);
            }
            else if (strcmp(p.we_wordv[0], "repeat") == 0)
            {
                char *temp = (char *)malloc(PATH_MAX);
                for (int j = 0; j < p.we_wordc - 2; j++)
                    strcat(temp, p.we_wordv[j + 2]);
                int repeat_times = atoi(p.we_wordv[1]);
                for (int i = 0; i < repeat_times; i++)
                {
                    parse(temp);
                }
            }
            else if (strcmp(p.we_wordv[0], "history") == 0)
            {
                write_history(p.we_wordc, p.we_wordv);
            }
            else if (strcmp(p.we_wordv[0], "ls") == 0)
                ls(p.we_wordc, p.we_wordv);
            else if (strcmp(p.we_wordv[0], "pwd") == 0)
                pwd();
            else if (strcmp(p.we_wordv[0], "echo") == 0)
                echo(inp);
            else if (strcmp(p.we_wordv[0], "pinfo") == 0)
            {
                if (p.we_wordc == 1)
                    pinfo(-1);
                else
                    pinfo(atoi(p.we_wordv[1]));
            }
            else if (strcmp(p.we_wordv[0], "exit") == 0)
                return -1;
            else
            {
                ex(p.we_wordv);
            }
        }
        token = strtok(NULL, ";");
    }
}
void promptandinput()
{
    char *inp = malloc(sizeof(char) * 100);
    char c;
    setbuf(stdout, NULL);
    enableRawMode();
    prompt();
    memset(inp, '\0', 100);
    int pt = 0;
    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        if (iscntrl(c))
        {
            if (c == 10)
            {
                printf("\n");
                break;
            }
            // else if (c == 27)
            // {
            //     int print_history = num_history;
            //     char buf[3];
            //     buf[2] = 0;
            //     if (read(STDIN_FILENO, buf, 2) == 2)
            //     { // length of escape code
            //         if (buf[0] == '[' && buf[1] == 'A')
            //         {
            //             print_history--;
            //             print_history < 0 ? print_history = 0 : print_history;
            //             printf("\r");
            //             prompt();
            //             printf("%s", history_array[print_history]);
            //         }
            //         else if (buf[0] == '[' && buf[1] == 'B')
            //         {
            //             print_history++;
            //             // print_history > num_history ? print_history = num_history : print_history;
            //             if (print_history > num_history)
            //             {
            //                 printf("\r");
            //                 prompt();
            //             }
            //             else
            //             {
            //                 printf("\r");
            //                 printf("%s", history_array[print_history]);
            //                 prompt();
            //             }
            //         }
            //     }
            // }
            else if (c == 127)
            { // backspace
                if (pt > 0)
                {
                    if (inp[pt - 1] == 9)
                    {
                        for (int i = 0; i < 7; i++)
                        {
                            printf("\b");
                        }
                    }
                    inp[--pt] = '\0';
                    printf("\b \b");
                }
            }
            else if (c == 9)
            { // TAB character
                inp[pt++] = c;
                for (int i = 0; i < 8; i++)
                { // TABS should be 8 spaces
                    printf(" ");
                }
            }
            else if (c == 4)
            {
                exit(0);
            }
            else
            {
                printf("%d\n", c);
            }
        }
        else
        {
            inp[pt++] = c;
            printf("%c", c);
        }
    }
    disableRawMode();
    add_history(inp);
    write_back();
    int parse_ret = parse(inp);
    if (parse_ret == -1)
    {
        exit(0);
        free(inp);
    }
}
int main()
{
    if (getcwd(HOME, PATH_MAX) != NULL)
    {
        ;
    }
    else
    {
        perror("getcwd() error");
        return 1;
    }
    init_history();
    signal(SIGCHLD, handle_sigquit);
    while (1)
    {
        promptandinput();
    }

    return 0;
}
