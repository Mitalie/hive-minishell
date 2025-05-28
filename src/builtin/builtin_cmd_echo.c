/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_echo.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:09:35 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/27 13:12:22 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

#include "env.h"
#include "status.h"
#include "util.h"
#include "libft.h"

/*
	Writes arguments with spaces between them
*/
static t_status	write_args(char **argv, int start_idx, int stdout_fd,
	int *exit_code)
{
	int	i;

	i = start_idx;
	while (argv[i])
	{
		if (!util_write_all(stdout_fd, argv[i], ft_strlen(argv[i])))
		{
			*exit_code = 1;
			return (S_COMM_ERR);
		}
		if (argv[i + 1])
		{
			if (!util_write_all(stdout_fd, " ", 1))
			{
				*exit_code = 1;
				return (S_COMM_ERR);
			}
		}
		i++;
	}
	return (S_OK);
}

/*
	echo builtin command - displays a line of text
*/
t_status	builtin_cmd_echo(char **argv, t_env *env,
	int *exit_code, int stdout_fd)
{
	bool		newline;
	int			i;
	t_status	status;

	*exit_code = 0;
	i = 1;
	(void)env;
	newline = true;
	if (argv[i] && ft_strncmp(argv[i], "-n", 3) == 0)
	{
		newline = false;
		i++;
	}
	status = write_args(argv, i, stdout_fd, exit_code);
	if (status != S_OK)
		return (status);
	if (newline && !util_write_all(stdout_fd, "\n", 1))
	{
		*exit_code = 1;
		return (S_COMM_ERR);
	}
	return (S_OK);
}
