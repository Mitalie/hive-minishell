/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_echo.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:09:35 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/30 21:53:14 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

#include "env.h"
#include "status.h"
#include "util.h"
#include "libft.h"

#include <stdio.h>

/*
	Writes arguments with spaces between them
*/
static bool	write_args(char **argv, int stdout_fd,
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
		{
			if (argv[i + 1])
				argv[i][len - 1] = '\0';
			return (false);
		}
		if (argv[i + 1])
			argv[i][len - 1] = '\0';
		i++;
	}
	return (true);
}

/*
	echo builtin command - displays a line of text
*/
t_status	builtin_cmd_echo(char **argv, t_env *env,
	int *exit_code, int stdout_fd)
{
	bool		newline;

	*exit_code = 0;
	(void)env;
	newline = true;
	argv++;
	if (*argv && ft_strncmp(*argv, "-n", 3) == 0)
	{
		newline = false;
		argv++;
	}
	if (!write_args(argv, stdout_fd, newline))
		*exit_code = 1;
	return (S_OK);
}
