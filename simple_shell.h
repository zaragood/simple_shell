#ifndef _SIMPLE_SHELL_H
#define _SIMPLE_SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

char *lineptr = NULL;
int _putchar(char c);
void get_user_input(void);
char **mystrtok(const char *delim);

#endif /*_SIMPLE_SHELL_H*/
