/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_filename.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:48:35 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/19 19:36:26 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word_internal.h"
#include "word.h"

#include <dirent.h>
#include <stdbool.h>
#include <stdlib.h>

#include "libft.h"

static void	word_filename_append(char *prefix, size_t prefix_len,
				char *value, struct s_word_field ***append);

/*
	Match a pattern against files in the current directory and append matching
	file names as fields to the given append pointer.

	TODO: handle opendir error
	TODO: handle readdir error
	TODO: report closedir error
*/
void	word_filename(char	*pattern_str, struct s_word_field ***matches_append,
	bool *had_matches)
{
	struct s_word_pattern	pattern;
	DIR						*dir;
	struct dirent			*dirent;

	*had_matches = false;
	if (!word_pattern_init_filename(&pattern, pattern_str))
		return ;
	dir = opendir(".");
	dirent = readdir(dir);
	while (dirent)
	{
		if (word_pattern_test_filename(&pattern, dirent->d_name))
		{
			word_filename_append(pattern.prefix, pattern.prefix_len,
				dirent->d_name, matches_append);
			*had_matches = true;
		}
		dirent = readdir(dir);
	}
	closedir(dir);
}

/*
	Allocate a new field for string and append it to the list.
*/
static void	word_filename_append(char *prefix, size_t prefix_len,
	char *value, struct s_word_field ***append)
{
	size_t				value_size;
	struct s_word_field	*field;

	value_size = ft_strlen(value) + 1;
	field = malloc(sizeof(*field) + prefix_len + value_size);
	field->next = NULL;
	ft_memcpy(field->value, prefix, prefix_len);
	ft_memcpy(field->value + prefix_len, value, value_size);
	**append = field;
	*append = &field->next;
}
