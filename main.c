#include "simple_shell.h"
#include "my_strtok.h"

/*
 * File: simple_shell.c
 * Description: A simple shell program that allows users to execute commands.
 * Author: Goodness Azara and Precious Nwosu
 * Date: July 19, 2023.
 */

/**
 * sigintHandler - Signal handler for SIGINT (Ctrl+C).
 * @sig: The signal number received (unused).
 * @lineptr: A pointer to the line buffer (unused).
 * Frees memory and exits the program when the signal is received.
 */
void sigintHandler(int sig, char **lineptr)
{
	(void) sig;
	free(*lineptr), *lineptr = NULL;
	exit(0);
}

/**
 * get_user_input - Reads user input from the command line.
 * Uses getline() to store the input in the lneptr variable.
 * Removes the newline character at the end
 * and handles possible errors
 * @lineptr: A pointer to a pointer to a character to store the user input.
 */
void get_user_input(char **lineptr)
{
	size_t n = 0;
	ssize_t nread;

	nread = getline(lineptr, &n, stdin);
	if (nread == -1)
	{
		free(*lineptr);
		*lineptr = NULL;
		exit(1);
	}
	/* Removes the newline character from the end of the input */
	(*lineptr)[nread - 1] = '\0';
}

/**
 * _which - Locates the executable path of a given command.
 * @env: An array of string containing environemt variables.
 * @command: The command whose executable path needs to be located.
 * Return: On success, returns a pointer to a string containing
 * the full path to the command. If the command is not found,
 * returns NULL.
 */
char *_which(char **env, char *command)
{
	unsigned int i = 0, j;
	char **paths = NULL, *buffer = NULL;
	size_t length = 0;
	struct stat statbuf;
	/* Find the 'PATH' environment variable in the provided 'env' */
	while (env[i] != NULL)
	{
		if (strncmp(env[i], "PATH=", 5) == 0)
			break;
		i++;
	}
	paths = my_strtok(":=", env[i]);

	if (paths)
	{
		for (i = 1; paths[i]; i++)
		{
			length = strlen(paths[i]) + strlen(command) + 2;
			buffer = malloc(length);
			if (buffer)
			{
				/* Copy the directory path to the buffer */
				for (length = 0; length < strlen(paths[i]); length++)
					buffer[length] = paths[i][length];
				/* Append '/' to the buffer */
				buffer[length++] = '/';

				for (j = 0; j < strlen(command); length++, j++)
					buffer[length] = command[j];

				buffer[length] = '\0';

				if (stat(buffer, &statbuf) == 0)
					return (buffer);
			}
			free(buffer);
		}
	}
	return (buffer);
}

/**
 * myfork - Forks a child process and executes the specified command.
 * @argv: The argumennts to pass to the command.
 * @av: The name of the program being executed (unused).
 * @environ: The environment variables.
 *
 * Return: 1 (failure)
 */
void myfork(char **argv, char **av, char **environ)
{
	pid_t child_pid;
	int status;
	struct stat statbuf;

	child_pid = fork();
	if (child_pid == -1)
		return;
	else if (child_pid == 0)
	{	/* Child process: Search for the executable in 'PATH' if necessary */
		if (stat(argv[0], &statbuf) != 0)
			argv[0] = _which(environ, argv[0]);
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

/**
 * main - Entry point of the simple shell program
 * @ac: The number of arguments (unused).
 * @av: The arguments (unused).
 * @environ: The environment variables.
 *
 * Return: Always 0.
 */
int main(int ac, char **av, char **environ)
{
	char *prompt = "#simple_shell$ ";
	int i;
	char **argv = NULL;
	bool interactive = isatty(fileno(stdin));
	char *lineptr = NULL;
	(void) ac;
/**
 * sigint_wrapper - Wrapper function for the SIGINT signal handler.
 * @sig: The signal number received (unused).
 */
	void sigint_wrapper(int sig)
	{
		sigintHandler(sig, &lineptr);
	}
	signal(SIGINT, sigint_wrapper);
	while (1)
	{
		if (interactive)
		{
			i = 0;
			while (prompt[i])
				_putchar(prompt[i++]);
		}
		get_user_input(&lineptr);
		argv = my_strtok(" ", lineptr);
		if (argv != NULL)
		{
			if (_strcmp(argv[0], "exit") == 0)
			{
				for (i = 0; argv[i]; i++)
					free(argv[i]), argv[i] = NULL;
				free(argv), argv = NULL;
				break;
			}
			myfork(argv, av, environ);
			for (i = 0; argv[i]; i++)
				free(argv[i]), argv[i] = NULL;
			free(argv), argv = NULL;
		}
		free(lineptr), lineptr = NULL;
	}
	free(lineptr), lineptr = NULL;
	return (0);
}
