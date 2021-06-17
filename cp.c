#include "shell.h"
#define MAXN 1024

int do_cp_file(char *src, char *dst)
{
    int fdr;
    struct stat statbuf;
    char buffer[MAXN];
    int n;
    int fd;
    if ((fd = open(src, O_RDONLY)) < 0)
    {
        return fd;
    }
    stat(src, &statbuf);
    if ((fdr = creat(dst, statbuf.st_mode)) < 0)
    {
        return fdr;
    }
    while ((n = read(fd, buffer, MAXN)) > 0)
    {
        if (write(fdr, buffer, n) != n)
        {
            close(fd);
            close(fdr);
            return 1;
        }
    }
    close(fd);
    close(fdr);
    return 0;
}

int do_cp_dir(char *src, char *dst)
{
    char source[MAXN];
    char target[MAXN];
    struct stat statbuf;
    DIR *dir;
    struct dirent *entry;
    if (lstat(dst, &statbuf) != 0)
    {
        mkdir(dst, statbuf.st_mode);
    }
    dir = opendir(src);
    if (dir == NULL)
    {
        return 1;
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == 4)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }
            sprintf(source, "%s/%s", src, entry->d_name);
            sprintf(target, "%s/%s", dst, entry->d_name);
            do_cp_dir(source, target);
        }
        else
        {
            sprintf(source, "%s/%s", src, entry->d_name);
            sprintf(target, "%s/%s", dst, entry->d_name);
            do_cp_file(source, target);
        }
    }
    closedir(dir);
    return 0;
}
int do_cp(char *src, char *dst)
{
    struct stat statbuf;
    DIR *dir;
    int result;
    if ((result = lstat(src, &statbuf)) != 0)
    {
        return result;
    }
    if (S_ISREG(statbuf.st_mode))
    {
        return do_cp_file(src, dst);
    }
    else if (S_ISDIR(statbuf.st_mode))
    {
        return do_cp_dir(src, dst);
    }
    else
    {
        return do_cp_file(src, dst);
    }
}

int cp(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("usage: cp <target> <dest> \n");
        return 1;
    }
    return do_cp(argv[1], argv[2]);
}
