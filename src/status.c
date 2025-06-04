/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 20:11:15 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/04 20:45:24 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "status.h"

#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "shenv.h"
#include "libft.h"
#include "shenv.h"
#include "util.h"

t_status	status_err(t_status status,
	const char *msg, const char *extra, int errnum)
{
	status_warn(msg, extra, errnum);
	return (status);
}

/*
	We want to avoid using malloc as this might be called after malloc failure.
	We should make the local buffer large enough for all the error messages we
	might print, but check to be safe.
*/
#define ERR_BUF_LEN 1000

/*
	Append to buffer and calculate new length.

	Using strlcpy avoids writing outside the buffer, but we can't blindly add
	the returned length as it might be larger than fits or even overflow the
	addition.

	By passing the buffer as pointer-to-array the compiler ensures the caller
	uses appropriate size.
*/
static size_t	status_msg_append(char (*buf)[ERR_BUF_LEN], size_t len,
	const char *str)
{
	size_t	added_len;

	added_len = ft_strlcpy(*buf + len, str, sizeof(*buf) - len);
	if (added_len < sizeof(*buf) - len)
		return (len + added_len);
	else
		return (sizeof(*buf) - 1);
}

void	status_warn(const char *msg, const char *extra, int errnum)
{
	char	buf[ERR_BUF_LEN];
	size_t	len;

	len = status_msg_append(&buf, 0, "minishell: ");
	len = status_msg_append(&buf, len, msg);
	if (extra)
	{
		len = status_msg_append(&buf, len, ": ");
		len = status_msg_append(&buf, len, extra);
	}
	if (errnum != 0)
	{
		len = status_msg_append(&buf, len, ": ");
		len = status_msg_append(&buf, len, strerror(errnum));
	}
	buf[len++] = '\n';
	util_write_all(STDERR_FILENO, buf, len);
}

void	status_set_exit_code(t_status status, t_shenv *env)
{
	if (status == S_EXIT_ERR || status == S_RESET_ERR || status == S_COMM_ERR
		|| status == S_BUILTIN_ERR)
		env->exit_code = 1;
	else if (status == S_RESET_SYNTAX || status == S_BUILTIN_ARG)
		env->exit_code = 2;
	else if (status == S_RESET_SIGINT)
		env->exit_code = 128 + SIGINT;
}

t_status	status_force_exit(t_status status, t_shenv *env)
{
	status_set_exit_code(status, env);
	return (S_EXIT_OK);
}
