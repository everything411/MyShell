#include "shell.h"

char *mode2str(int mode)
{
    static char str[16];
    strcpy(str, "----------");
    if (S_ISDIR(mode))
    {
        str[0] = 'd';
    }
    if (S_ISCHR(mode))
    {
        str[0] = 'c';
    }
    if (S_ISBLK(mode))
    {
        str[0] = 'b';
    }
    if ((mode & S_IRUSR))
    {
        str[1] = 'r';
    }
    if ((mode & S_IWUSR))
    {
        str[2] = 'w';
    }
    if ((mode & S_IXUSR))
    {
        str[3] = 'x';
    }
    if ((mode & S_IRGRP))
    {
        str[4] = 'r';
    }
    if ((mode & S_IWGRP))
    {
        str[5] = 'w';
    }
    if ((mode & S_IXGRP))
    {
        str[6] = 'x';
    }
    if ((mode & S_IROTH))
    {
        str[7] = 'r';
    }
    if ((mode & S_IWOTH))
    {
        str[8] = 'w';
    }
    if ((mode & S_IXOTH))
    {
        str[9] = 'x';
    }
    return str;
}

char *uid2name(uid_t uid)
{
    struct passwd *pw_ptr;
    static char tmp_str[10];
    if ((pw_ptr = getpwuid(uid)) == NULL)
    {
        sprintf(tmp_str, "%d", uid);
        return tmp_str;
    }
    else
    {
        return pw_ptr->pw_name;
    }
}

char *gid2Name(gid_t gid)
{
    struct group *grp_ptr;
    static char tmp_str[10];
    if ((grp_ptr = getgrgid(gid)) == NULL)
    {
        sprintf(tmp_str, "%d", gid);
        return tmp_str;
    }
    else
    {
        return grp_ptr->gr_name;
    }
}
void print_file_info(char *filename, struct stat *info_p, int mode)
{
    if (!((mode >> 0) & 1) && filename[0] == '.')
    {
        return;
    }
    else
    {
        if ((mode >> 1) & 1)
        {
            printf("%s", mode2str(info_p->st_mode));
            printf(" %4d", (int)info_p->st_nlink);
            printf(" %-8s", uid2name(info_p->st_uid));
            printf(" %-8s", gid2Name(info_p->st_gid));
            printf(" %8ld", (long)info_p->st_size);
            printf(" %.12s", 4 + ctime(&info_p->st_mtime));
            printf(" %s\n", filename);
        }
        else
        {
            printf(" %s\n", filename);
        }
    }
}
int do_ls(char *dir, int mode)
{
    DIR *dir_ptr;
    struct dirent *direntp;
    if ((dir_ptr = opendir(dir)) == NULL)
    {
        return 1;
    }
    else
    {
        while ((direntp = readdir(dir_ptr)) != NULL)
        {
            struct stat info;
            if (stat(direntp->d_name, &info) == -1)
            {
                return 1;
            }
            else
            {
                print_file_info(direntp->d_name, &info, mode);
            }
        }
        closedir(dir_ptr);
        return 0;
    }
}

int ls(int argc, char *argv[])
{
    if (argc == 1)
    {
        puts("usage: ls -[a][l] [dir]");
        return 1;
    }
    int mode = 0;
    int ch;
    while ((ch = getopt(argc, argv, "al")) != -1)
    {
        switch (ch)
        {
        case 'a':
            mode |= 1;
            break;
        case 'l':
            mode |= 2;
            break;
        default:
            break;
        }
    }
    const char *dir;
    if ((!mode && argc == 1) || (mode && argc == 2))
    {
        do_ls(".", mode);
    }
    else if (!mode)
    {
        for (int i = 1; i < argc; i++)
        {
            do_ls(argv[i], mode);
        }
    }
    else
    {
        for (int i = 2; i < argc; i++)
        {
            do_ls(argv[i], mode);
        }
    }
}