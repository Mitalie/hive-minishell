/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_unset.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 23:45:02 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/30 10:05:12 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

#include <unistd.h>

#include "env.h"
#include "status.h"
#include "util.h"
#include "libft.h"
#include "builtin_cmd_export_utils.h"

/*
	Unsets a single environment variable
*/
static t_status	unset_single_var(char *var, t_env *env, int *exit_code)
{
	t_status	status;

	if (!is_valid_identifier(var))
	{
		status_warn("unset: not a valid identifier", var, 0);
		*exit_code = 2;
		return (S_OK);
	}
	status = env_unset(env, var);
	if (status != S_OK)
	{
		*exit_code = 1;
		return (status);
	}
	return (S_OK);
}

/*
	unset builtin command - removes environment variables
*/
t_status	builtin_cmd_unset(char **argv, t_env *env,
	int *exit_code, int stdout_fd)
{
	int			i;
	t_status	status;

	(void)stdout_fd;
	*exit_code = 0;
	if (!argv[1])
		return (S_OK);
	i = 1;
	while (argv[i])
	{
		status = unset_single_var(argv[i], env, exit_code);
		if (status != S_OK)
			return (status);
		i++;
	}
	execve("/bin/env", (char *[]){"/bin/env", NULL}, env->env_array);
	return (S_OK);
}
