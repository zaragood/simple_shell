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


extern char *lineptr;
int _putchar(char c);
int _strcmp(char *s1, char *s2);
#endif /*_SIMPLE_SHELL_H*/
