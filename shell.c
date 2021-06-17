#include "shell.h"
char *arglist[MAXN];
char history[MAXN][MAXN];
int cmd_cnt = 0;
int argcc = 0;
int flag;

int exec(char **argv)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        return 1;
    }
    else if (pid == 0)
    {
        if (execvp(argv[0], argv))
        {
            perror(argv[0]);
        }
        return 0;
    }
    else
    {
        int wstatus;
        if (waitpid(pid, &wstatus, 0) == -1)
        {
            return 1;
        }
        return wstatus;
    }
}

int main(int argc, char **argv)
{
    char buf[MAXN];
    int pid;
    int result;
    char *delim = " \b\r\n\t";
    cmd_cnt = 0;
    memset(history, 0x00, sizeof(history));
    while (1)
    {
        printf("$ ");
        fflush(stdout);
        argcc = 0;
        memset(arglist, 0x00, sizeof(arglist));
        if (!fgets(buf, BUFFSIZE, stdin))
        {
            break;
        }
        strcpy(history[cmd_cnt++], buf);
        char *p;
        p = strtok(buf, delim);
        if (!p)
        {
            continue;
        }
        arglist[argcc++] = p;
        while ((p = strtok(NULL, delim)))
        {
            arglist[argcc++] = p;
        }
        arglist[argcc] = NULL;
        // for (int i = 0; i < argcc; i++)
        // {
        //     printf("argv[%d]='%s'\n", i, arglist[i]);
        // }

        if (strcmp(arglist[0], "exit") == 0)
        {
            exit(0);
        }
        else if (strcmp(arglist[0], "pwd") == 0)
        {
            char buf[LEN];
            getcwd(buf, sizeof(buf));
            printf("%s\n", buf);
        }
        else if (strcmp(arglist[0], "cd") == 0)
        {
            if (cd(argcc, arglist))
            {
                perror(arglist[0]);
            }
        }
        else if (strcmp(arglist[0], "cp") == 0)
        {
            if (cp(argcc, arglist))
            {
                perror(arglist[0]);
            }
        }
        else if (strcmp(arglist[0], "ls") == 0)
        {
            if (ls(argcc, arglist))
            {
                perror(arglist[0]);
            }
        }
        else if (strcmp(arglist[0], "history") == 0)
        {
            for (int i = 0; i < cmd_cnt; i++)
            {
                printf("%s", history[i]);
            }
        }
        else if (strcmp(arglist[0], "rm") == 0)
        {
            if (rm(argcc, arglist))
            {
                perror(arglist[0]);
            }
        }
        else if (strcmp(arglist[0], "mv") == 0)
        {
            if (mv(argcc, arglist))
            {
                perror(arglist[0]);
            }
        }
        else
        {
            if (exec(arglist))
            {
                perror(argv[0]);
            }
        }
    }
    return 0;
}