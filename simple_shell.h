#ifndef _SIMPLE_SHELL_H
#define _SIMPLE_SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <signal.h>
#include <ctype.h>

char *lineptr = NULL;
int _putchar(char c);
void myfork(char **argv, char **av, char **environ);
#endif /*_SIMPLE_SHELL_H*/
