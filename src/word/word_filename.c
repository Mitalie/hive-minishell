/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_filename.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:48:35 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/11 19:06:46 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word_internal.h"
#include "word.h"

#include <dirent.h>
#include <stdbool.h>
#include <stdlib.h>

#include "libft.h"

static bool	word_filename_matches(char *value, char *pattern);
static void	word_filename_append(char *value, struct s_word_field ***append);

/*
	Match a pattern against files in the current directory and append matching
	file names as fields to the given append pointer.

	TODO: handle opendir error
	TODO: handle readdir error
	TODO: report closedir error
*/
void	word_filename(char	*pattern, struct s_word_field ***matches_append,
	bool *had_matches)
{
	DIR					*dir;
	struct dirent		*dirent;

	*had_matches = false;
	dir = opendir(".");
	dirent = readdir(dir);
	while (dirent)
	{
		if (word_filename_matches(dirent->d_name, pattern))
		{
			word_filename_append(dirent->d_name, matches_append);
			*had_matches = true;
		}
		dirent = readdir(dir);
	}
	closedir(dir);
}

/*
	Test whether a string matches a pattern.

	TODO: implement real pattern matching, this is just for testing
*/
static bool	word_filename_matches(char *value, char *pattern)
{
	if (ft_strncmp(value, pattern, 2) == 0)
		return (true);
	return (false);
}

/*
	Allocate a new field for string and append it to the list.
*/
static void	word_filename_append(char *value, struct s_word_field ***append)
{
	size_t				value_size;
	struct s_word_field	*field;

	value_size = ft_strlen(value) + 1;
	field = malloc(sizeof(*field) + value_size);
	field->next = NULL;
	ft_memcpy(field->value, value, value_size);
	**append = field;
	*append = &field->next;
}
