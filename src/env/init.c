/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 01:34:15 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/14 18:15:38 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "env_internal.h"

#include <stdlib.h>

#include "libft.h"

extern char	**environ;

static t_status	init_env_array(t_env *env, int size)
{
	if (size > 0)
		env->array_size = size * 2;
	else
		env->array_size = 10;
	env->used_size = 0;
	env->env_array = malloc(sizeof(char *) * (env->array_size + 1));
	if (!env->env_array)
		return (status_err(S_RESET_ERR, "malloc", NULL, 0));
	env->env_array[0] = NULL;
	return (S_OK);
}

static t_status	copy_environ_to_env(t_env *env)
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
		status = env_resize(env);
		if (status != S_OK)
		{
			free(value);
			return (status);
		}
		env->env_array[env->used_size++] = value;
		env->env_array[env->used_size] = NULL;
		i++;
	}
	return (S_OK);
}

t_status	env_init(t_env *env)
{
	t_status	status;
	size_t		count;

	count = 0;
	while (environ && environ[count])
		count++;
	status = init_env_array(env, count);
	if (status != S_OK)
		return (status);
	status = copy_environ_to_env(env);
	if (status != S_OK)
	{
		env_free(env);
		return (status);
	}
	return (S_OK);
}

void	env_free(t_env *env)
{
	size_t	i;

	if (!env)
		return ;
	if (env->env_array)
	{
		i = 0;
		while (i < env->used_size)
		{
			free(env->env_array[i]);
			i++;
		}
		free(env->env_array);
	}
}
