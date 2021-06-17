#include "shell.h"

#define tpDIRECTORY 4
#define tpFILE 8
#define tpLINK 10

int do_cp_file(char *src, char *dst);
int do_cp_dir(char *src, char *dst);
int do_rm_dir(char *target, int mode);
int do_rm_file(char *target);

int do_mv_file(char *src, char *dst)
{
    if (!rename(src, dst))
    {
        return 0;
    }
    else
    {
        if (do_cp_file(src, dst))
        {
            return 1;
        }
        if (do_rm_file(src))
        {
            return 1;
        }
        return 0;
    }
}
int do_mv_dir(char *src, char *dst)
{
    if (!rename(src, dst))
    {
        return 0;
    }
    else
    {
        if (do_cp_dir(src, dst))
        {
            return 1;
        }
        if (do_rm_dir(src, 1))
        {
            return 1;
        }
        return 0;
    }
}

int do_mv(char *src, char *dst)
{
    struct stat st;
    if (lstat(src, &st))
    {
        printf("lstat error\n");
        return 2;
    }

    if (S_ISREG(st.st_mode))
    {
        return do_mv_file(src, dst);
    }
    else if (S_ISDIR(st.st_mode))
    {
        return do_mv_dir(src, dst);
    }
    else
    {
        return do_mv_file(src, dst);
    }
}

int mv(int argc, char *argv[])
{
    if (argc != 3)
    {
        puts("usage: mv source destination");
        return 2;
    }
    return do_mv(argv[1], argv[2]);
}
