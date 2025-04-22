/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 02:15:43 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/21 18:47:07 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "env_internal.h"
#include "libft.h"
#include <stdlib.h>

char	*env_get(t_env *env, const char *key)
{
	int		index;
	size_t	key_len;

	index = env_find_index(env, key);
	if (index == -1)
		return (NULL);
	key_len = ft_strlen(key);
	return (env->env_array[index] + key_len + 1);
}

static int	update_existing_entry(t_env *env, int index, char *new_entry)
{
	free(env->env_array[index]);
	env->env_array[index] = new_entry;
	return (0);
}

static int	add_new_entry(t_env *env, char *new_entry)
{
	if (env_resize(env) == -1)
	{
		free(new_entry);
		return (-1);
	}
	env->env_array[env->meta.used_size] = new_entry;
	env->meta.used_size++;
	env->env_array[env->meta.used_size] = NULL;
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
		return (update_existing_entry(env, index, new_entry));
	return (add_new_entry(env, new_entry));
}

int	env_unset(t_env *env, const char *key)
{
	int	index;

	index = env_find_index(env, key);
	if (index == -1)
		return (0);
	free(env->env_array[index]);
	ft_memmove(&env->env_array[index], &env->env_array[index + 1],
		sizeof(char *) * (env->meta.used_size - index));
	env->meta.used_size--;
	env->env_array[env->meta.used_size] = NULL;
	return (0);
}
