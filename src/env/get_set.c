/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 02:15:43 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/01 21:32:01 by josmanov         ###   ########.fr       */
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

	key_len = ft_strlen(key);
	index = env_find_index(env, key, key_len);
	if (index == -1)
		return (NULL);
	return (env->env_array[index] + key_len + 1);
}

static t_status	update_existing_entry(t_env *env, int index, char *new_entry)
{
	free(env->env_array[index]);
	env->env_array[index] = new_entry;
	return (S_OK);
}

static t_status	add_new_entry(t_env *env, char *new_entry)
{
	t_status	status;

	status = env_resize(env);
	if (status != S_OK)
	{
		free(new_entry);
		return (status);
	}
	env->env_array[env->meta.used_size] = new_entry;
	env->meta.used_size++;
	env->env_array[env->meta.used_size] = NULL;
	return (S_OK);
}

t_status	env_set(t_env *env, const char *key, const char *value)
{
	int		index;
	char	*new_entry;
	size_t	key_len;
	size_t	value_len;

	key_len = ft_strlen(key);
	value_len = ft_strlen(value);
	index = env_find_index(env, key, key_len);
	new_entry = create_env_string(key, key_len, value, value_len);
	if (!new_entry)
		return (status_err(S_RESET_ERR, "malloc", NULL, 0));
	if (index != -1)
		return (update_existing_entry(env, index, new_entry));
	return (add_new_entry(env, new_entry));
}

t_status	env_unset(t_env *env, const char *key)
{
	int		index;
	size_t	key_len;

	key_len = ft_strlen(key);
	index = env_find_index(env, key, key_len);
	if (index == -1)
		return (S_OK);
	free(env->env_array[index]);
	ft_memmove(&env->env_array[index], &env->env_array[index + 1],
		sizeof(char *) * (env->meta.used_size - index));
	env->meta.used_size--;
	env->env_array[env->meta.used_size] = NULL;
	return (S_OK);
}
