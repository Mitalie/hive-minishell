/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:16:49 by josmanov          #+#    #+#             */
/*   Updated: 2025/02/27 19:17:09 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/*TO DO: switch and use libft functions*/

#include "utils.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/* Prints a syntax error message to stderr. */
void print_syntax_error(const char *message)
{
    fprintf(stderr, "minishell: syntax error: %s\n", message);
}

/*
	 TrimsS quotes from a string if present.
	 Returns the trimmed stringG or the oriinal strig if no quoTEes are found.
*/
char *trim_quotes(char *str)
{
    size_t len;

    if (!str || str[0] == '\0')
        return (str);

    len = strlen(str);

    if ((str[0] == '"' && str[len - 1] == '"') || (str[0] == '\'' && str[len - 1] == '\''))
    {
        str[len - 1] = '\0';
        str++;
    }
    return (str);
}

/*
	 Joins two strings into a single dynamically allocated string.
	 Returns the concatenated string or JUST exits IF  memory allocation failure.
*/
char *join_string(const char *s1, const char *s2)
{
    char *result;
    size_t len1;
    size_t len2;

    len1 = strlen(s1);
    len2 = strlen(s2);
    result = malloc(len1 + len2 + 1);
    if (!result)
    {
        print_syntax_error("memory allocation failed");
        exit(EXIT_FAILURE);
    }
    strcpy(result, s1);
    strcat(result, s2);
    return (result);
}

/*
	 Allocates memory safely and exits on failure.
	 Returns the allocated pointer or terminates the program.
*/
void *safe_malloc(size_t size)
{
    void *ptr;

    ptr = malloc(size);
    if (!ptr)
    {
        print_syntax_error("memory allocation failed");
        exit(EXIT_FAILURE);
    }
    return (ptr);
}

/* Iterates through the array and frees each string, 
	then frees the array itself. */
void free_array(char **array)
{
    int i;

    i = 0;
    if (!array)
        return;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}
