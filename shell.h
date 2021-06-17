#ifndef _SHELL_H
#define _SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <fcntl.h>
#include <dirent.h>
#include <utime.h>
#include <grp.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

#define MAXN 1024
#define BUFFSIZE 1024
#define LEN 128

int cd(int argc, char *argv[]);
int cp(int argc, char *argv[]);
int ls(int argc, char *argv[]);
int rm(int argc, char *argv[]);

#endif