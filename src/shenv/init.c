/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 01:34:15 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/04 21:18:45 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shenv.h"
#include "shenv_internal.h"

#include <stdlib.h>

#include "libft.h"

extern char	**environ;

static t_status	init_env_array(t_shenv *env, int size)
{
	if (size > 0)
		env->var_array_size = size * 2;
	else
		env->var_array_size = 10;
	env->var_array_used = 0;
	env->var_array = malloc(sizeof(char *) * (env->var_array_size + 1));
	if (!env->var_array)
		return (status_err(S_RESET_ERR, "malloc", NULL, 0));
	env->var_array[0] = NULL;
	return (S_OK);
}

static t_status	copy_environ_to_env(t_shenv *env)
{
	int			i;
	char		*value;
	t_status	status;

	if (!environ)
		return (S_OK);
	i = 0;
	while (environ[i])
	{
		value = ft_strdup(environ[i]);
		if (!value)
			return (status_err(S_EXIT_ERR, ERRMSG_MALLOC, NULL, 0));
		status = shenv_var_array_resize(env);
		if (status != S_OK)
		{
			free(value);
			return (status);
		}
		env->var_array[env->var_array_used++] = value;
		env->var_array[env->var_array_used] = NULL;
		i++;
	}
	return (S_OK);
}

t_status	shenv_init(t_shenv *env)
{
	t_status	status;
	size_t		count;

	env->exit_code = 0;
	count = 0;
	while (environ && environ[count])
		count++;
	status = init_env_array(env, count);
	if (status != S_OK)
		return (status);
	status = copy_environ_to_env(env);
	if (status != S_OK)
	{
		shenv_free(env);
		return (status);
	}
	return (S_OK);
}

void	shenv_free(t_shenv *env)
{
	size_t	i;

	if (!env)
		return ;
	if (env->var_array)
	{
		i = 0;
		while (i < env->var_array_used)
		{
			free(env->var_array[i]);
			i++;
		}
		free(env->var_array);
	}
}
