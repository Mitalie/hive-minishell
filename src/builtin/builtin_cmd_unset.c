/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_unset.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 23:45:02 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/04 20:14:39 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

#include "shenv.h"
#include "status.h"

/*
	unset builtin command - removes environment variables
*/
t_status	builtin_cmd_unset(char **argv, t_shenv *env,
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
		status = shenv_var_unset(env, argv[i]);
		if (status != S_OK)
			return (status);
		i++;
	}
	return (S_OK);
}
