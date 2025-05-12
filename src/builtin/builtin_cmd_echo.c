/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_echo.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:09:35 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/12 20:49:03 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

#include "env.h"
#include "status.h"
#include "util.h"
#include "libft.h"

/*
	TO DO: 
	
	* Add error handling for util_write_all functions
	* Add handling for the case when no arguments are provided
*/
t_status	builtin_cmd_echo(char **argv, t_env *env,
	int *exit_code, int stdout_fd)
{
	bool	newline;
	int		i;

	*exit_code = 0;
	i = 1;
	(void)env;
	newline = true;
	if (argv[i] && ft_strncmp(argv[i], "-n", 3) == 0)
	{
		newline = false;
		i++;
	}
	while (argv[i])
	{
		util_write_all(stdout_fd, argv[i], ft_strlen(argv[i]));
		if (argv[i + 1])
			util_write_all(stdout_fd, " ", 1);
		i++;
	}
	if (newline)
		util_write_all(stdout_fd, "\n", 1);
	return (S_OK);
}
