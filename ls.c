#include "ls.h"
#include "headers.h"

void ls_l(char *path, char exclude_dot, char print_folder_name);

void ls_normal(char *path, char exclude_dot, char print_folder_name)
{
    struct stat s;
    struct dirent *dir_info;
    if (stat(path, &s) != 0)
    {
        perror("ls");
        return;
    }
    if (s.st_mode & S_IFDIR)
    {
        if (print_folder_name)
            printf("%s:\n", path);

        DIR *dir = opendir(path);
        if (dir == NULL)
        {
            RED
                perror("ls");
            WHITE
            return;
        }
        else
        {
            WHITE
            while ((dir_info = readdir(dir)) != NULL)
            {
                if (dir_info->d_name[0] != exclude_dot)
                    printf("%s\n", dir_info->d_name);
            }
        }
    }
    else
    {
        printf("%s\n", basename(path));
    }
    printf("\n");
}

void ls(int argc, char *argv[])
{
    char print_folder_name = 0;
    char exclude_dot = '.';
    char call_l = 0;
    char *path = (char *)calloc(PATH_MAX, 1);
    // if (argc == 1)
    // {
    //     argv[1] = ".";
    // }
    int opt = 0, no_of_option = 0;
    optind = 0;
    while ((opt = getopt(argc, argv, ":al")) != -1)
    {
        no_of_option++;
        switch (opt)
        {
        case 'a':
            exclude_dot = '\0';
            break;
        case 'l':
            call_l = 1;
            break;
        case '?':
            printf("ls: Unknown parameter");
            return;
        }
    }
    if (argc - optind > 1)
        print_folder_name = 1;
    if (optind < argc)
    {
        for (; optind < argc; optind++)
        {
            if (argv[optind] == NULL)
                strcpy(path, ".");
            else
                strcpy(path, argv[optind]);

            if (call_l)
                ls_l(path, exclude_dot, print_folder_name);
            else
            {
                ls_normal(path, exclude_dot, print_folder_name);
            }
        }
    }
    else if (argc == optind)
    {
        if (call_l)
            ls_l(".", exclude_dot, print_folder_name);
        else
        {
            ls_normal(".", exclude_dot, print_folder_name);
        }
    }
    free(path);
}

void ls_l(char *path, char exclude_dot, char print_folder_name)
{
    char file_name[PATH_MAX];
    if (path[strlen(path) - 1] == '/')
        path[strlen(path) - 1] = '\0';
    struct stat s;
    if (stat(path, &s) != 0)
    {
        RED
            printf("ls");
        WHITE
    }

    if (s.st_mode & S_IFDIR)
    {
        if (print_folder_name)
            printf("%s:\n", path);

        DIR *dir = opendir(path);
        if (dir == NULL)
        {
            RED
                perror("ls");
            WHITE
            return;
        }
        else
        {
            time_t TIME = time(NULL);
            struct tm *T = localtime(&TIME);
            struct dirent *dir_info;
            char buffer[100], b[100];
            WHITE
            int max_len;
            int block_size = 0;
            while ((dir_info = readdir(dir)) != NULL)
            {
                if (dir_info->d_name[0] != exclude_dot)
                {
                    file_name[0] = '\0';
                    strcat(file_name, path);
                    strcat(file_name, "/");
                    strcat(file_name, dir_info->d_name);
                    stat(file_name, &s);
                    block_size += (s.st_blocks) / 2;
                    sprintf(buffer, "%ld", s.st_size);
                    if (strlen(buffer) > max_len)
                        max_len = strlen(buffer);
                }
            }
            printf("Total %d\n", block_size);
            dir = opendir(path);
            while ((dir_info = readdir(dir)) != NULL)
            {
                if (dir_info->d_name[0] != exclude_dot)
                {
                    file_name[0] = '\0';
                    strcat(file_name, path);
                    strcat(file_name, "/");
                    strcat(file_name, dir_info->d_name);
                    stat(file_name, &s);
                    printf("%c", s.st_mode & S_IFDIR ? 'd' : '-');
                    printf("%c", s.st_mode & S_IRUSR ? 'r' : '-');
                    printf("%c", s.st_mode & S_IWUSR ? 'w' : '-');
                    printf("%c", s.st_mode & S_IXUSR ? 'x' : '-');
                    printf("%c", s.st_mode & S_IRGRP ? 'r' : '-');
                    printf("%c", s.st_mode & S_IWGRP ? 'w' : '-');
                    printf("%c", s.st_mode & S_IXGRP ? 'x' : '-');
                    printf("%c", s.st_mode & S_IROTH ? 'r' : '-');
                    printf("%c", s.st_mode & S_IWOTH ? 'w' : '-');
                    printf("%c ", s.st_mode & S_IXOTH ? 'x' : '-');
                    printf("%ld ", s.st_nlink);
                    struct passwd *pw = getpwuid(s.st_uid);
                    printf("%s ", pw->pw_name);
                    struct group *gr = getgrgid(s.st_gid);
                    printf("%s ", gr->gr_name);

                    buffer[0] = '\0';
                    b[0] = '%';
                    b[1] = '\0';
                    sprintf(buffer, "%d", max_len);
                    strcat(b, buffer);
                    strcat(b, "ld ");
                    printf(b, s.st_size);

                    TIME = time(NULL);
                    // TIME = TIME - s.st_mtime;
                    T = localtime(&s.st_mtime);
                    strftime(buffer, 100, "%b %d %H:%M", T);
                    printf("%s %s\n", buffer, basename(file_name));
                }
            }
        }
    }
    else
    {
        char buffer[100], b[100];
        stat(file_name, &s);
        printf("%c", s.st_mode & S_IFDIR ? 'd' : '-');
        printf("%c", s.st_mode & S_IRUSR ? 'r' : '-');
        printf("%c", s.st_mode & S_IWUSR ? 'w' : '-');
        printf("%c", s.st_mode & S_IXUSR ? 'x' : '-');
        printf("%c", s.st_mode & S_IRGRP ? 'r' : '-');
        printf("%c", s.st_mode & S_IWGRP ? 'w' : '-');
        printf("%c", s.st_mode & S_IXGRP ? 'x' : '-');
        printf("%c", s.st_mode & S_IROTH ? 'r' : '-');
        printf("%c", s.st_mode & S_IWOTH ? 'w' : '-');
        printf("%c ", s.st_mode & S_IXOTH ? 'x' : '-');
        printf("%ld ", s.st_nlink);
        struct passwd *pw = getpwuid(s.st_uid);
        printf("%s ", pw->pw_name);
        struct group *gr = getgrgid(s.st_gid);
        printf("%s ", gr->gr_name);
        buffer[0] = '\0';
        printf("%ld ", s.st_size);
        struct tm *T = localtime(&s.st_mtime);
        strftime(buffer, 100, "%b %d %H:%M", T);
        printf("%s %s\n", buffer, basename(path));
    }
    printf("\n");
}