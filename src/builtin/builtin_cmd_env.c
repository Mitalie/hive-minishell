/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_env.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 23:15:02 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/27 14:59:14 by josmanov         ###   ########.fr       */
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
	size_t	len;
	char	*s;

	(void)argv;
	*exit_code = 0;
	i = 0;
	while (i < env->used_size)
	{
		s = env->env_array[i];
		len = ft_strlen(s);
		s[len] = '\n';
		if (!util_write_all(stdout_fd, s, len + 1))
		{
			s[len] = '\0';
			*exit_code = 1;
			return (S_COMM_ERR);
		}
		s[len] = '\0';
		i++;
	}
	return (S_OK);
}
