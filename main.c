#include "simple_shell.h"

/*
 * File: simple_shell.c
 * Description: A simple shell program that allows users to execute commands.
 * Author: Goodness Azara and Precious Nwosu
 * Date: July 19, 2023.
 */

/**
 * sigint_handler - Signal handler for SIGINT (Ctrl+C).
 * @sig: The signal number received (unused).
 * Frees memory and exits the program when the signal is received.
 */
void sigint_handler(int sig)
{
	(void) sig;
	free(lineptr),
	lineptr = NULL;
	exit(0);
}

/**
 * get_user_input - Reads user input from the command line.
 * Uses getline() to store the input in the lneptr variable.
 * Removes the newline character at the end
 * and handles possible errors
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
 * my_strtok - Custom implementation of strtok function
 * to split the input line.
 * @delim: The delimiter to split the input line.
 * @buffer: The input line to be tokenized.
 *
 * Splits the line stored in lineptr based on the given delimiter.
 * Return: An array of tokens (strings) obtained from
 * the split operation.
 */
char **my_strtok(const char *delim, char *buffer)
{
	char *token = NULL, *line;
	char **tokens;
	int i = 0;

	/* Duplicate the line stored in lineptr */
	line = strdup(buffer);

	/* Allocate memory for the array of tokens */
	tokens = malloc(sizeof(char *) * 1024);
	if (!tokens)
	{
		return (NULL);
	}
	/* Split the line into tokens using strtok */
	token = strtok(line, delim);
	if (token == NULL)
	{
		free(line), line = NULL;
		free(tokens), tokens = NULL;
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
				{
					free(paths);
					return (buffer);
				}
			}
			free(buffer);
		}
		free(paths);
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
	char *path = NULL;
	struct stat statbuf;

	child_pid = fork();
	if (child_pid == -1)
	{
		return;
	}
	else if (child_pid == 0)
	{	/* Child process: Search for the executable in 'PATH' if necessary */
		if (stat(argv[0], &statbuf) != 0)
			path = _which(environ, argv[0]);
		if (path)
		{
			free(argv[0]);
			argv[0] = path;
		}
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
 * Return: Always 0.
 */
int main(int ac, char **av, char **environ)
{
	char *prompt = "#simple_shell$ ";
	char **argv = NULL;
	int i;
	bool interactive = isatty(fileno(stdin));
	(void) ac;

	signal(SIGINT, sigint_handler);
	while (1)
	{
		if (interactive)
		{
			i = 0;
			while (prompt[i])
				_putchar(prompt[i++]);
		}
		get_user_input();
		argv = my_strtok(" ", lineptr);
		if (argv != NULL)
		{
			if (strcmp(argv[0], "exit") == 0)
			{
				free(lineptr), lineptr = NULL;
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
	return (0);
}
