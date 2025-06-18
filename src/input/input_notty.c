/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_notty.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 16:38:04 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/18 18:15:20 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_internal.h"

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "libft.h"
#include "signals.h"
#include "status.h"
#include "util.h"

/*
	Reallocate the buffer, copying data from the previous allocation. If new
	allocation fails, old allocation remains.
*/
static t_status	input_notty_realloc(char **buf, size_t new_sz, size_t old_sz)
{
	char	*new_buf;

	new_buf = malloc(new_sz);
	if (!new_buf)
		return (status_err(S_EXIT_ERR, ERRMSG_MALLOC, NULL, 0));
	if (*buf)
	{
		if (old_sz > new_sz)
			old_sz = new_sz;
		ft_memcpy(new_buf, *buf, old_sz);
	}
	free(*buf);
	*buf = new_buf;
	return (S_OK);
}

#define BUF_INIT_SIZE 256

/*
	Ensure buffer has space for one more byte, reallocating if necessary.
*/
static t_status	input_notty_grow_buf(char **buf, size_t *buf_sz, size_t buf_len)
{
	if (buf_len < *buf_sz)
		return (S_OK);
	if (*buf_sz > SIZE_MAX / 2)
		return (status_err(S_EXIT_ERR, "input line too long", NULL, 0));
	if (*buf_sz == 0)
		*buf_sz = BUF_INIT_SIZE;
	else
		*buf_sz *= 2;
	return (input_notty_realloc(buf, *buf_sz, buf_len));
}

/*
	Read a single character from standard input.
*/
static t_status	input_notty_read_char(char *buf_pos, bool *eof_read)
{
	ssize_t	res;

	while (true)
	{
		res = read(STDIN_FILENO, buf_pos, 1);
		if (res < 0 && errno == EINTR)
		{
			if (signals_check_sigint(true))
				return (S_RESET_SIGINT);
			continue ;
		}
		if (res < 0)
			return (status_err(S_EXIT_ERR, "input error",
					"read() failed", errno));
		if (res == 0)
			*eof_read = true;
		return (S_OK);
	}
}

/*
	Grow buffer and read characters from standard input until end-of-file is
	encountered or a newline character or a null byte is read. If anything was
	read, reallocates buffer down to actual size and terminates it with NULL.
*/
static t_status	input_notty_read_line(char **buf, bool *read_any)
{
	t_status	status;
	size_t		buf_sz;
	size_t		line_len;
	bool		eof_read;

	status = S_OK;
	buf_sz = 0;
	line_len = 0;
	eof_read = false;
	while (status == S_OK && !eof_read)
	{
		status = input_notty_grow_buf(buf, &buf_sz, line_len);
		if (status == S_OK)
			status = input_notty_read_char(*buf + line_len, &eof_read);
		if (status != S_OK || eof_read)
			break ;
		*read_any = true;
		if ((*buf)[line_len] == '\n')
			(*buf)[line_len] = '\0';
		if ((*buf)[line_len++] == '\0')
			break ;
	}
	if (status == S_OK && *read_any)
		status = input_notty_realloc(buf, line_len, line_len);
	return (status);
}

#define ERRMSG_ECHO "input echo failed: write() failed"

/*
	Read a line from standard input as a NULL terminated string. If nothing is
	read (EOF condition), a null pointer is stored instead of an empty string.
*/
t_status	input_notty_get_line(char **line_out, const char *prompt)
{
	t_status	status;
	bool		read_any;
	char		*echo;

	*line_out = NULL;
	read_any = false;
	status = input_notty_read_line(line_out, &read_any);
	if (status == S_OK)
	{
		if (read_any)
			echo = *line_out;
		else
			echo = "*EOF*";
		if (!util_write_all(STDOUT_FILENO, prompt, ft_strlen(prompt))
			|| !util_write_all(STDOUT_FILENO, echo, ft_strlen(echo))
			|| !util_write_all(STDOUT_FILENO, "\n", 1))
			status = status_err(S_EXIT_ERR, ERRMSG_ECHO, NULL, errno);
	}
	if (status != S_OK || !read_any)
	{
		free(*line_out);
		*line_out = NULL;
	}
	return (status);
}
