/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 02:15:43 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/13 14:44:25 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "env_internal.h"
#include <stdlib.h>
#include <string.h>

char	*env_get(t_env *env, const char *key)
{
	int		index;
	size_t	key_len;

	index = env_find_index(env, key);
	if (index == -1)
		return (NULL);
	key_len = strlen(key);
	return (env->env_array[index] + key_len + 1);
}

static int	handle_existing_entry(t_env *env, int index, char *new_entry)
{
	char	*temp;

	if (index >= env->original_size)
		free(env->env_array[index]);
	env->env_array[index] = new_entry;
	if (index < env->original_size)
	{
		if (env->original_size > 1)
		{
			temp = env->env_array[index];
			env->env_array[index] = env->env_array[env->original_size - 1];
			env->env_array[env->original_size - 1] = temp;
			index = env->original_size - 1;
		}
		env->original_size--;
	}
	return (0);
}

static int	add_new_entry(t_env *env, char *new_entry)
{
	if (env_resize(env) == -1)
	{
		free(new_entry);
		return (-1);
	}
	env->env_array[env->used_size] = new_entry;
	env->used_size++;
	env->env_array[env->used_size] = NULL;
	return (0);
}

int	env_set(t_env *env, const char *key, const char *value)
{
	int		index;
	char	*new_entry;

	index = env_find_index(env, key);
	new_entry = create_env_string(key, value);
	if (!new_entry)
		return (-1);
	if (index != -1)
	{
		if (handle_existing_entry(env, index, new_entry) == -1)
		{
			free(new_entry);
			return (-1);
		}
	}
	else
	{
		if (add_new_entry(env, new_entry) == -1)
			return (-1);
	}
	return (0);
}

int	env_unset(t_env *env, const char *key)
{
	int	index;
	int	i;

	index = env_find_index(env, key);
	if (index == -1)
		return (0);
	if (index >= env->original_size)
		free(env->env_array[index]);
	i = index;
	while (i < env->used_size - 1)
	{
		env->env_array[i] = env->env_array[i + 1];
		i++;
	}
	env->used_size--;
	env->env_array[env->used_size] = NULL;
	return (0);
}
