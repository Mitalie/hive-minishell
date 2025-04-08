/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_filename.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:33:54 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/08 18:53:59 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word_internal.h"
#include "word.h"

#include <dirent.h>
#include <stdbool.h>
#include <stdlib.h>

#include "libft.h"

/*
	TODO: implement properly, this is just for testing
*/
static bool	word_filename_matches(char *filename, char *pattern)
{
	if (ft_strncmp(filename, pattern, 2) == 0)
		return (true);
	return (false);
}

/*
	TODO: rename or move elsewhere
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

/*
	Find files in the current working directory that match the given pattern.

	TODO: handle opendir error
	TODO: handle readdir error
	TODO: report closedir error
*/
struct s_word_field	*word_filename(char *pattern)
{
	DIR					*dir;
	struct dirent		*dirent;
	struct s_word_field	*matches;
	struct s_word_field	**append_ptr;

	matches = NULL;
	append_ptr = &matches;
	dir = opendir(".");
	dirent = readdir(dir);
	while (dirent)
	{
		if (word_filename_matches(dirent->d_name, pattern))
			word_filename_append(dirent->d_name, &append_ptr);
		dirent = readdir(dir);
	}
	closedir(dir);
	return (matches);
}
