/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_env.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 23:15:02 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/03 15:05:32 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

#include <errno.h>

#include "env.h"
#include "libft.h"
#include "status.h"
#include "util.h"

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
			return (status_err(S_BUILTIN_ERR, "env", "write failed", errno));
		}
		s[len] = '\0';
		i++;
	}
	return (S_OK);
}
