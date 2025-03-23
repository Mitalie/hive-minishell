/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:23:02 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/23 14:49:04 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <stdlib.h>
#include <string.h>

/*
    Trims quotes from a string if present.
    Returns a new string without quotes or NULL if allocation fails.
*/
char	*trim_quotes(char *str)
{
	size_t	len;
	char	*result;

	if (!str || str[0] == '\0')
		return (str);
	len = strlen(str);
	if ((str[0] == '"' && str[len - 1] == '"') 
		|| (str[0] == '\'' && str[len - 1] == '\''))
	{
		result = malloc(len - 1);
		if (!result)
			return (NULL);
		strncpy(result, str + 1, len - 2);
		result[len - 2] = '\0';
		return (result);
	}
	return (strdup(str));
}

/*
    Joins two strings into a single dynamically allocated string.
    Returns the concatenated string or NULL on failure.
*/
char	*join_string(const char *s1, const char *s2)
{
	char	*result;
	size_t	len1;
	size_t	len2;

	len1 = strlen(s1);
	len2 = strlen(s2);
	result = malloc(len1 + len2 + 1);
	if (!result)
		return (NULL);
	strcpy(result, s1);
	strcat(result, s2);
	return (result);
}

/*
    Allocates memory safely and returns NULL on failure.
    Returns the allocated pointer.
*/
void	*safe_malloc(size_t size)
{
	return (malloc(size));
}

/* 
    Iterates through the array and frees each string,
    then frees the array itself.
*/
void	free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void free_tokens(struct s_token *tokens)
{
    if (!tokens)
        return;

    int i = 0;
    while (tokens[i].type != TOK_END)
    {
        if (tokens[i].word_content)
            free(tokens[i].word_content);
        i++;
    }

    free(tokens);
}
