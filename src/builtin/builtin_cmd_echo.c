/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_echo.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:09:35 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/03 15:20:39 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

#include <errno.h>

#include "env.h"
#include "libft.h"
#include "status.h"
#include "util.h"

/*
	Writes arguments with spaces between them
*/
static t_status	write_args(char **argv, int stdout_fd,
	bool newline_for_last)
{
	int		i;
	size_t	len;

	i = 0;
	while (argv[i])
	{
		len = ft_strlen(argv[i]);
		if (argv[i + 1])
			argv[i][len++] = ' ';
		else if (newline_for_last)
			argv[i][len++] = '\n';
		if (!util_write_all(stdout_fd, argv[i], len))
			return (status_err(S_BUILTIN_ERR, "echo", "write failed", errno));
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
	bool	newline;

	*exit_code = 0;
	(void)env;
	newline = true;
	argv++;
	if (*argv && ft_strncmp(*argv, "-n", 3) == 0)
	{
		newline = false;
		argv++;
	}
	return (write_args(argv, stdout_fd, newline));
}
