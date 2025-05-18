/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_env.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 23:15:02 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/18 06:03:49 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

#include "env.h"
#include "status.h"
#include "util.h"
#include "libft.h"

/*
	Prints all environment variables in the format KEY=VALUE
*/
t_status	builtin_cmd_env(char **argv, t_env *env,
	int *exit_code, int stdout_fd)
{
	size_t	i;

	(void)argv;
	*exit_code = 0;
	i = 0;
	while (i < env->used_size)
	{
		util_write_all(stdout_fd, env->env_array[i],
			ft_strlen(env->env_array[i]));
		util_write_all(stdout_fd, "\n", 1);
		i++;
	}
	return (S_OK);
}
