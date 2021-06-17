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
        exit(execvp(argv[0], argv));
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        return status;
    }
}

int main(int argc, char **argv)
{
    char buf[MAXN];
    int pid;
    int result;
    cmd_cnt = 0;
    memset(history, 0x00, sizeof(history));
    while (1)
    {
        argcc = 0;
        memset(arglist, 0x00, sizeof(arglist));
        printf("$ ");
        fflush(stdout);
        fgets(buf, BUFFSIZE, stdin);

        if (strcmp(buf, "\n") == 0)
        {
            printf("\n");
            continue;
        }
        strcpy(history[cmd_cnt], buf);
        cmd_cnt++;
        memset(arglist, 0x00, sizeof(arglist));

        char *delim = " \b\r\n\t";
        char *p;
        p = strtok(buf, delim);
        if (!p)
        {
            continue;
        }
        arglist[argc++] = p;
        while ((p = strtok(NULL, delim)))
        {
            arglist[argc++] = p;
        }

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
            cd(argcc, arglist);
        }
        else if (strcmp(arglist[0], "cp") == 0)
        {
            cp(argcc, arglist);
        }
        else if (strcmp(arglist[0], "ls") == 0)
        {
            myls(argcc, arglist);
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
            rm(argcc, arglist);
        }
        else if (strcmp(arglist[0], "mv") == 0)
        {
            mv(argcc, arglist);
        }
        else
        {
            exec(arglist);
        }
    }
    return 0;
}