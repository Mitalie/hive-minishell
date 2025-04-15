/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 01:34:15 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/13 14:35:06 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "env_internal.h"
#include <stdlib.h>
#include <stddef.h>

extern char	**environ;

static int	init_env_array(t_env *env, int size)
{
	if (size > 0)
		env->array_size = size * 2;
	else
		env->array_size = 10;
	env->used_size = size;
	env->original_size = size;
	env->env_array = malloc(sizeof(char *) * (env->array_size + 1));
	if (!env->env_array)
		return (0);
	return (1);
}

t_env	*env_init(void)
{
	t_env	*env;
	int		i;
	int		size;

	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	size = count_array_size(environ);
	if (!init_env_array(env, size))
	{
		free(env);
		return (NULL);
	}
	i = 0;
	while (i < size)
	{
		env->env_array[i] = environ[i];
		i++;
	}
	env->env_array[i] = NULL;
	return (env);
}

void	env_free(t_env *env)
{
	int	i;

	if (!env)
		return ;
	i = env->original_size;
	while (i < env->used_size)
	{
		free(env->env_array[i]);
		i++;
	}
	free(env->env_array);
	free(env);
}
