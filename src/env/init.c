/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 01:34:15 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/21 18:51:46 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "env_internal.h"
#include "libft.h"
#include <stdlib.h>

extern char	**environ;

static int	init_env_array(t_env *env, int size)
{
	if (size > 0)
		env->meta.array_size = size * 2;
	else
		env->meta.array_size = 10;
	env->meta.used_size = 0;
	env->env_array = malloc(sizeof(char *) * (env->meta.array_size + 1));
	if (!env->env_array)
		return (0);
	env->env_array[0] = NULL;
	return (1);
}

static char	*ft_strdup_env(const char *s)
{
	char	*new;
	size_t	len;

	len = ft_strlen(s);
	new = malloc(len + 1);
	if (!new)
		return (NULL);
	ft_memcpy(new, s, len + 1);
	return (new);
}

static int	fill_env_array(t_env *env)
{
	int		i;
	char	*value;

	i = 0;
	while (environ[i])
	{
		value = ft_strdup_env(environ[i]);
		if (!value || env_resize(env) == -1)
		{
			env_free(env);
			return (-1);
		}
		env->env_array[env->meta.used_size++] = value;
		env->env_array[env->meta.used_size] = NULL;
		i++;
	}
	return (0);
}

t_env	*env_init(void)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	if (!init_env_array(env, count_array_size(environ)))
	{
		free(env);
		return (NULL);
	}
	if (fill_env_array(env) == -1)
		return (NULL);
	return (env);
}

void	env_free(t_env *env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (i < env->meta.used_size)
	{
		free(env->env_array[i]);
		i++;
	}
	free(env->env_array);
	free(env);
}
