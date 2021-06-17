#include "shell.h"

int do_rm_file(char *target)
{
    return remove(target);
}
int do_rm_dir(char *target, int mode)
{
    if (!mode)
    {
        return remove(target);
    }
    else
    {
        DIR *dir;
        struct dirent *entry;
        char dst[MAXN];
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_type == 4)
            {
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                {
                    continue;
                }
                sprintf(dst, "%s/%s", target, entry->d_name);
                do_rm_dir(dst, mode);
            }
            else
            {
                sprintf(dst, "%s/%s", target, entry->d_name);
                do_rm_file(dst);
            }
        }
        closedir(dir);
    }
}

int do_rm(char *target, int mode)
{
    struct stat statbuf;
    int result;
    if ((result = lstat(target, &statbuf)) != 0)
    {
        return result;
    }
    if (S_ISREG(statbuf.st_mode))
    {
        return do_rm_file(target);
    }
    else if (S_ISDIR(statbuf.st_mode))
    {
        return do_rm_dir(target, mode);
    }
    else
    {
        return do_rm_file(target);
    }

    return 0;
}

int rm(int argc, char *argv[])
{
    if (argc == 1)
    {
        puts("usage: rm [-r] file-or-directory");
        return 1;
    }
    int mode = 0;
    int ch;
    while ((ch = getopt(argc, argv, "r")) != -1)
    {
        switch (ch)
        {
        case 'r':
            mode |= 1;
            break;
        default:
            break;
        }
    }
    const char *dir;
    if (!mode)
    {
        for (int i = 1; i < argc; i++)
        {
            do_rm(argv[i], mode);
        }
    }
    else
    {
        for (int i = 2; i < argc; i++)
        {
            do_rm(argv[i], mode);
        }
    }
    return 0;
}
