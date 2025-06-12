/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_filename.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:48:35 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/12 16:39:42 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word_internal.h"
#include "word.h"

#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#include "libft.h"

#include "status.h"

static t_status	word_filename_append(char *prefix, size_t prefix_len,
					char *value, struct s_word_field ***append);
static t_status	word_filename_scandir(DIR *dir, struct s_word_pattern *pattern,
					struct s_word_field ***append, bool *had_matches);

#define OPENDIR_ERRMSG "could not open current directory for filename matching"
#define READDIR_ERRMSG "could not read directory for filename matching"

/*
	Linux closedir can only fail with invalid argument so we don't handle error.
	POSIX also allows EINTR, but a) we should use SA_RESTART, and b) it likely
	has same problem as error from close, where the file descriptor may be
	closed already and retrying would be a mistake.
*/

/*
	Match a pattern against files in the current directory and append matching
	file names as fields to the given append pointer.

	`had_matches` is set to true if and only if any fields are appended, even in
	case of error.
*/
t_status	word_filename(char *pattern_str,
	struct s_word_field ***matches_append, bool *had_matches)
{
	struct s_word_pattern	pattern;
	DIR						*dir;
	t_status				status;

	*had_matches = false;
	if (!word_pattern_init_filename(&pattern, pattern_str))
		return (S_OK);
	dir = opendir(".");
	if (!dir)
		return (status_err(S_RESET_ERR, OPENDIR_ERRMSG, NULL, errno));
	status = word_filename_scandir(dir, &pattern, matches_append, had_matches);
	closedir(dir);
	return (status);
}

static t_status	word_filename_scandir(DIR *dir, struct s_word_pattern *pattern,
	struct s_word_field ***matches_append, bool *had_matches)
{
	struct dirent	*dirent;
	t_status		status;
	bool			match;

	while (true)
	{
		errno = 0;
		dirent = readdir(dir);
		if (!dirent && errno)
			return (status_err(S_RESET_ERR, READDIR_ERRMSG, NULL, errno));
		if (!dirent)
			return (S_OK);
		status = word_pattern_test_filename(pattern, dirent->d_name, &match);
		if (status != S_OK)
			return (status);
		if (match)
		{
			status = word_filename_append(pattern->prefix, pattern->prefix_len,
					dirent->d_name, matches_append);
			if (status != S_OK)
				return (status);
			*had_matches = true;
		}
	}
}

/*
	Allocate a new field for string and append it to the list.
*/
static t_status	word_filename_append(char *prefix, size_t prefix_len,
	char *value, struct s_word_field ***append)
{
	size_t				value_size;
	struct s_word_field	*field;

	value_size = ft_strlen(value) + 1;
	field = malloc(sizeof(*field) + prefix_len + value_size);
	if (!field)
		return (status_err(S_EXIT_ERR, ERRMSG_MALLOC, NULL, 0));
	field->next = NULL;
	ft_memcpy(field->value, prefix, prefix_len);
	ft_memcpy(field->value + prefix_len, value, value_size);
	**append = field;
	*append = &field->next;
	return (S_OK);
}
