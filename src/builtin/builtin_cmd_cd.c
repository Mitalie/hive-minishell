/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_cd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 22:30:02 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/05 00:02:21 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

#include <errno.h>
#include <unistd.h>

#include "shenv.h"
#include "status.h"

/*
	Determines the target path for cd command
*/
static t_status	get_cd_path(char **argv, t_shenv *env, char **path)
{
	if (!argv[1])
	{
		*path = shenv_var_get(env, "HOME");
		if (!*path)
			return (status_err(S_BUILTIN_ERR, "cd", "HOME not set", 0));
	}
	else if (argv[2])
		return (status_err(S_BUILTIN_ARG, "cd", "too many arguments", 0));
	else
		*path = argv[1];
	return (S_OK);
}

/*
	cd builtin command - changes the current working directory
*/
t_status	builtin_cmd_cd(char **argv, t_shenv *env, int stdout_fd)
{
	t_status	status;
	char		*path;

	(void)stdout_fd;
	env->exit_code = 0;
	status = get_cd_path(argv, env, &path);
	if (status != S_OK)
		return (status);
	if (chdir(path) != 0)
		return (status_err(S_BUILTIN_ERR, "cd", path, errno));
	return (S_OK);
}
