/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_unset.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 23:45:02 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/03 16:42:49 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

#include "env.h"
#include "status.h"

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
		status = env_unset(env, argv[i]);
		if (status != S_OK)
			return (status);
		i++;
	}
	return (S_OK);
}
