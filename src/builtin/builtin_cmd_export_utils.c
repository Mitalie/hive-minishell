/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd_export_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 23:30:02 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/18 06:03:14 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

#include <stdlib.h>
#include <unistd.h>

#include "env.h"
#include "status.h"
#include "util.h"
#include "libft.h"
#include "builtin_cmd_export_utils.h"

/*
	Prints all environment variables in the format "declare -x KEY="VALUE""
	For variables without values, prints just "declare -x KEY"
*/
void	print_exports(t_env *env, int stdout_fd)
{
	size_t	i;
	char	*equal_sign;

	i = 0;
	while (i < env->used_size)
	{
		util_write_all(stdout_fd, "declare -x ", 11);
		equal_sign = ft_strchr(env->env_array[i], '=');
		if (equal_sign)
		{
			util_write_all(stdout_fd, env->env_array[i],
				equal_sign - env->env_array[i]);
			util_write_all(stdout_fd, "=\"", 2);
			util_write_all(stdout_fd, equal_sign + 1,
				ft_strlen(equal_sign + 1));
			util_write_all(stdout_fd, "\"", 1);
		}
		else
			util_write_all(stdout_fd, env->env_array[i],
				ft_strlen(env->env_array[i]));
		util_write_all(stdout_fd, "\n", 1);
		i++;
	}
}

/*
	Checks if a string is a valid shell identifier
	Must start with a letter or underscore (not a digit)
	Can only contain letters, digits, or underscores
	Cannot be empty
*/
bool	is_valid_identifier(const char *str)
{
	if (!str || !*str)
		return (false);
	if (!util_isname(*str) || util_isdigit(*str))
		return (false);
	str++;
	while (*str && *str != '=')
	{
		if (!util_isname(*str))
			return (false);
		str++;
	}
	return (true);
}

/*
	Sets an environment variable and frees the key and value strings
*/
t_status	set_env_var(char *key, char *value, t_env *env)
{
	t_status	status;

	status = env_set(env, key, value);
	free(key);
	free(value);
	return (status);
}
