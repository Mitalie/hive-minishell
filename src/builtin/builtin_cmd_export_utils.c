/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_export_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 23:30:02 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/04 20:14:39 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_cmd_export_utils.h"

#include <errno.h>
#include <stdlib.h>

#include "libft.h"
#include "shenv.h"
#include "status.h"
#include "util.h"

#define PREFIX "declare -x "

/*
	Helper function to print a single export line. Escapes any double quotes in
	the environment value. Surrounds value with double quotes.
*/
static t_status	print_single_export(char *env_str, char *buffer, int stdout_fd)
{
	size_t	out_pos;
	char	c;
	bool	has_value;

	out_pos = ft_strlen(PREFIX);
	ft_memcpy(buffer, PREFIX, out_pos);
	has_value = false;
	while (*env_str)
	{
		c = *env_str++;
		if (c == '"')
			buffer[out_pos++] = '\\';
		buffer[out_pos++] = c;
		if (c == '=' && !has_value)
		{
			has_value = true;
			buffer[out_pos++] = '"';
		}
	}
	if (has_value)
		buffer[out_pos++] = '"';
	buffer[out_pos++] = '\n';
	if (!util_write_all(stdout_fd, buffer, out_pos))
		return (status_err(S_BUILTIN_ERR, "export", "write failed", errno));
	return (S_OK);
}

/*
	Ensures buffer is large enough, reallocates if needed
*/
static t_status	ensure_buffer_size(char **buffer, size_t *buffer_size,
	size_t needed_len)
{
	if (*buffer && needed_len <= *buffer_size)
		return (S_OK);
	free(*buffer);
	*buffer_size = needed_len + 256;
	*buffer = malloc(*buffer_size);
	if (!*buffer)
		return (status_err(S_EXIT_ERR, ERRMSG_MALLOC, NULL, 0));
	return (S_OK);
}

/*
	Processes a single environment variable for export printing
	Count the length of the variable, plus extra space to escape any double
	quotes. Add the prefix length, plus 1 for newline at the end, plus 2 for
	quotes around the value. If variable is not valid (no equal sign/value),
	just print it with prefix.
*/
static t_status	process_export_entry(char *env_str, char **buffer,
	size_t *buffer_size, int stdout_fd)
{
	t_status	status;
	char		*equal_sign;
	size_t		needed_len;

	needed_len = 0;
	while (env_str[needed_len])
	{
		if (env_str[needed_len] == '"')
			needed_len++;
		needed_len++;
	}
	needed_len += ft_strlen(PREFIX) + 1;
	equal_sign = ft_strchr(env_str, '=');
	if (equal_sign)
		needed_len += 2;
	status = ensure_buffer_size(buffer, buffer_size, needed_len);
	if (status != S_OK)
		return (status);
	return (print_single_export(env_str, *buffer, stdout_fd));
}

/*
	Prints all environment variables in the format "declare -x KEY="VALUE""
	For variables without values, prints just "declare -x KEY"
*/
t_status	print_exports(t_shenv *env, int stdout_fd)
{
	t_status	status;
	size_t		i;
	char		*buffer;
	size_t		buffer_size;

	buffer_size = 1024;
	buffer = NULL;
	i = 0;
	status = S_OK;
	while (i < env->var_array_used && status == S_OK)
	{
		status = process_export_entry(env->var_array[i], &buffer,
				&buffer_size, stdout_fd);
		i++;
	}
	free(buffer);
	return (status);
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
