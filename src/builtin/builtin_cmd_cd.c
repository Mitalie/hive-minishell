/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_cd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 22:30:02 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/18 06:10:31 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include "env.h"
#include "status.h"
#include "util.h"
#include "libft.h"

/*
	Determines the target path for cd command
*/
static char	*get_cd_path(char **argv, t_env *env, int *exit_code)
{
	char	*path;

	if (!argv[1])
	{
		path = env_get(env, "HOME");
		if (!path)
		{
			status_warn("cd", "HOME not set", 0);
			*exit_code = 2;
			return (NULL);
		}
	}
	else if (argv[2])
	{
		status_warn("cd", "too many arguments", 0);
		*exit_code = 2;
		return (NULL);
	}
	else
		path = argv[1];
	return (path);
}

/*
	cd builtin command - changes the current working directory
	Used temporary /bin/pwd for testing
*/
t_status	builtin_cmd_cd(char **argv, t_env *env,
	int *exit_code, int stdout_fd)
{
	char	*path;

	(void)stdout_fd;
	*exit_code = 0;
	path = get_cd_path(argv, env, exit_code);
	if (!path)
		return (S_COMM_ERR);
	if (chdir(path) != 0)
	{
		status_err(S_COMM_ERR, "cd", path, errno);
		*exit_code = 2;
		return (S_COMM_ERR);
	}
	execve("/bin/pwd", (char *[]){"/bin/pwd", NULL}, env->env_array);
	return (S_OK);
}
