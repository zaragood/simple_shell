#include <stdlib.h>
#include <string.h>

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
