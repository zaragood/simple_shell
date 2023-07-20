#include "simple_shell.h"

/*
 * File: simple_shell.c
 * Description: A simple shell program that allows users to execute commands.
 * Author: Goodness Azara and Precious Nwosu
 * Date: July 19, 2023.
 */

char *lineptr = NULL;

/**
 * Signal handler for SIGINT (Ctrl+C).
 * Frees memory and exits the program when the signal is received.
 */
void sigintHandler(int sig)
{
	(void) sig;
	free(lineptr), lineptr = NULL;
	exit(0);
}

/**
 * Reads user input from the command line.
 * Uses getline() to store the input in the lneptr variable.
 * Removes the newline character at the end and handles possible errors
 */
void get_user_input(void)
{
	size_t n = 0;
	ssize_t nread;

	nread = getline(&lineptr, &n, stdin);
	if (nread == -1)
	{
		free(lineptr);
		lineptr = NULL;
		exit(1);
	}
	/* Removes the newline character from the end of the input */
	lineptr[nread - 1] = '\0';
}

/**
 * Custom implementation of strtok function to split the input line.
 * Splits the line stored in lineptr based on the given delimiter.
 * Returns an array of tokens (strings) obtained from the split operation.
 */
char **my_strtok(const char *delim)
{
	char *token = NULL, *line;
	char **tokens;
	int i = 0;

	/* Duplicate the line stored in lineptr */
	line = strdup(lineptr);

	/* Allocate memory for the array of tokens */
	tokens = malloc(sizeof(char *) * 10);
	if (!tokens)
	{
		free(line), line = NULL;
		return (NULL);
	}
	/* Split the line into tokens using strtok */
	token = strtok(line, delim);
	if (token == NULL)
	{
		free(line);
		line = NULL;
		free(tokens);
		tokens = NULL;
		return (NULL);
	}

	while (token)
	{
		/* Duplicate the token and store it in the tokens array */
		tokens[i] = strdup(token);
		token = strtok(NULL, delim);
		i++;
	}

	tokens[i] = NULL;
	free(line), line = NULL;
	return (tokens);
}

/**
 * Forks a child process and executes the specified command.
 * If the child process fails to execute the command, an error message is displayed.
 */
void myfork(char **argv, char **av, char **environ)
{
	pid_t child_pid;
	int status;

	child_pid = fork();
	if (child_pid == -1)
		return;
	else if (child_pid == 0)
	{
		/* Child process: Execute the command using execve */
		if (execve(argv[0], argv, environ) == -1)
			perror(av[0]);
	}
	else
	{
		/* Parent process: Wait for the child process to finish */
		wait(&status);
	}
}

int main(int ac, char **av, char **environ)
{
	char *prompt = "#simple_shell$ ";
	int i;
	char **argv = NULL;
	bool interactive = isatty(fileno(stdin));

	(void) ac;

	/* Set the signal handler for SIGINT (Ctrl+C) */
	signal(SIGINT, sigintHandler);

	while (1)
	{
		if (interactive)
		{
			i = 0;
			while (prompt[i])
				_putchar(prompt[i++]);
		}

		get_user_input();
		argv = my_strtok(" ");

		if (argv)
		{
			if (_strcmp(argv[0], "exit") == 0)
			{
				for (i = 0; argv[i]; i++)
				{
					free(argv[i]);
					argv[i] = NULL;
				}
				free(argv), argv = NULL;
				break;
			}
			/* Execute the entered command */
			myfork(argv, av, environ);

			/* Free memory allocated for the arguments */
			for (i = 0; argv[i]; i++)
			{
				free(argv[i]), argv[i] = NULL;
			}
			free(argv);
			argv = NULL;
		}
		/* Free memory allocated for the user input */
		free(lineptr);
		lineptr = NULL;
	}
	/* Free memory allocated for lineptr before exiting the program */
	free(lineptr);
	lineptr = NULL;

	return (0);
}
