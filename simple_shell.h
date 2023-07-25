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

void get_user_input(char **lineptr);
void sigint_wrapper(int sig);
char **my_strtok(const char *delim, char *buffer);
int _putchar(char c);
int _strcmp(char *s1, char *s2);

#endif /*_SIMPLE_SHELL_H*/
