/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_filename.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:33:54 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/04 19:16:18 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word_internal.h"

#include <dirent.h>
#include <stdlib.h>

#include "libft.h"

/*
	TODO: implement properly, this is just for testing
*/
bool	word_filename_matches(char *filename, char *pattern)
{
	if (ft_strncmp(filename, pattern, 2) == 0)
		return (true);
	return (false);
}

/*
	TODO: rename or move elsewhere
*/
void	word_fields_append(char *value, struct s_word_field ***append_ptr)
{
	size_t				value_size;
	struct s_word_field	*field;

	value_size = ft_strlen(value) + 1;
	field = malloc(sizeof(*field) + value_size);
	ft_memcpy(field->value, value, value_size);
	**append_ptr = field;
	*append_ptr = &field->next;
}

/*
	Iterate through directory entries in current working directory, and store
	any that match the given filename pattern.

	TODO: handle opendir error
	TODO: handle readdir error
	TODO: report closedir error
*/
void	word_filename_scan(char *pattern, struct s_word_field ***append_ptr)
{
	DIR					*dir;
	struct dirent		*dirent;

	dir = opendir(".");
	dirent = readdir(dir);
	while (dirent)
	{
		if (word_filename_matches(dirent->d_name, pattern))
			word_fields_append(dirent->d_name, append_ptr);
		dirent = readdir(dir);
	}
	closedir(dir);
}

/*
	Interpret the field as a filename pattern, and search for matches inside
	current working directory. If matches are found, replace the pattern field
	in its linked list with the matches. Otherwise keep the pattern field and
	perform quote removal on its content.
*/
void	word_filename(struct s_word_field **pattern, struct s_word_field ***lastnext)
{
	struct s_word_field	*matches;
	struct s_word_field	**append_ptr;

	matches = NULL;
	append_ptr = &matches;
	word_filename_scan((*pattern)->value, &append_ptr);
	if (matches)
	{
		*append_ptr = (*pattern)->next;
		*lastnext = append_ptr;
		free(*pattern);
		*pattern = matches;
	}
	else
		word_unquote((*pattern)->value);
}
