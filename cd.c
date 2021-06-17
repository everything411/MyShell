#include "shell.h"
#define LEN 128

int cd(int argc, char *argv[])
{
    if (argc > 2)
    {
        printf("cd: too many arguments\n");
        return 1;
    }
    const char *dir;
    if (argc == 1)
    {
        if ((dir = getenv("HOME")) == NULL)
        {
            dir = getpwuid(getuid())->pw_dir;
        }
    }
    else
    {
        dir = argv[1];
    }
    if (chdir(dir) < 0)
    {
        printf("cd: No such file or directory", dir);
        return 1;
    }
    printf("\n");
}
