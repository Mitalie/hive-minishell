/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_export_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 23:30:02 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/30 08:05:28 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

#include <stdlib.h>
#include <unistd.h>

#include "env.h"
#include "status.h"
#include "util.h"
#include "libft.h"
#include "builtin_cmd_export_utils.h"

/*
	Helper function to print a single export line
*/
static bool	print_single_export(char *env_str, char *equal_sign,
	char *buffer, int stdout_fd)
{
	size_t	pos;

	pos = 0;
	ft_memcpy(buffer + pos, "declare -x ", 11);
	pos += 11;
	if (equal_sign)
	{
		ft_memcpy(buffer + pos, env_str, equal_sign - env_str);
		pos += equal_sign - env_str;
		ft_memcpy(buffer + pos, "=\"", 2);
		pos += 2;
		ft_memcpy(buffer + pos, equal_sign + 1, ft_strlen(equal_sign + 1));
		pos += ft_strlen(equal_sign + 1);
		buffer[pos++] = '"';
	}
	else
	{
		ft_memcpy(buffer + pos, env_str, ft_strlen(env_str));
		pos += ft_strlen(env_str);
	}
	buffer[pos++] = '\n';
	return (util_write_all(stdout_fd, buffer, pos));
}

/*
	Ensures buffer is large enough, reallocates if needed
*/
static char	*ensure_buffer_size(char *buffer, size_t *buffer_size,
	size_t needed_len)
{
	char	*new_buffer;

	if (needed_len <= *buffer_size)
		return (buffer);
	free(buffer);
	*buffer_size = needed_len + 256;
	new_buffer = malloc(*buffer_size);
	return (new_buffer);
}

/*
	Processes a single environment variable for export printing
	prefix_len value 11 = "declare -x"
	quotes_len value 3 = "..."
*/
static bool	process_export_entry(char *env_str, char **buffer,
	size_t *buffer_size, int stdout_fd)
{
	char	*equal_sign;
	size_t	needed_len;
	size_t	prefix_len;
	size_t	quotes_len;

	equal_sign = ft_strchr(env_str, '=');
	prefix_len = 11;
	quotes_len = 3;
	if (equal_sign)
		needed_len = prefix_len + (equal_sign - env_str)
			+ quotes_len + ft_strlen(equal_sign + 1);
	else
		needed_len = prefix_len + ft_strlen(env_str) + 1;
	*buffer = ensure_buffer_size(*buffer, buffer_size, needed_len);
	if (!*buffer)
		return (false);
	return (print_single_export(env_str, equal_sign, *buffer, stdout_fd));
}

/*
	Prints all environment variables in the format "declare -x KEY="VALUE""
	For variables without values, prints just "declare -x KEY"
*/
bool	print_exports(t_env *env, int stdout_fd)
{
	size_t	i;
	char	*buffer;
	size_t	buffer_size;

	buffer_size = 1024;
	buffer = malloc(buffer_size);
	if (!buffer)
		return (false);
	i = 0;
	while (i < env->used_size)
	{
		if (!process_export_entry(env->env_array[i], &buffer,
				&buffer_size, stdout_fd))
		{
			free(buffer);
			return (false);
		}
		i++;
	}
	free(buffer);
	return (true);
}

/*
	Checks if a string is a valid shell identifier
	Must start with a letter or underscore (not a digit)
	Can only contain letters, digits, or underscores
	Cannot be empty
*/
bool	is_valid_identifier(const char *str)
{
	if (!str || !*str)
		return (false);
	if (!util_isname(*str) || util_isdigit(*str))
		return (false);
	str++;
	while (*str)
	{
		if (!util_isname(*str))
			return (false);
		str++;
	}
	return (true);
}
