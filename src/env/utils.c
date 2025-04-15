/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 01:54:36 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/13 14:31:50 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "env_internal.h"
#include <stdlib.h>
#include <string.h>

int	count_array_size(char **array)
{
	int	count;

	count = 0;
	while (array && array[count])
		count++;
	return (count);
}

int	env_find_index(t_env *env, const char *key)
{
	int		i;
	size_t	key_len;

	key_len = strlen(key);
	i = 0;
	while (i < env->used_size)
	{
		if (strncmp(env->env_array[i], key, key_len) == 0
			&& env->env_array[i][key_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

char	*create_env_string(const char *key, const char *value)
{
	char	*env_str;
	size_t	key_len;
	size_t	value_len;

	key_len = strlen(key);
	value_len = strlen(value);
	env_str = malloc(key_len + value_len + 2);
	if (!env_str)
		return (NULL);
	strcpy(env_str, key);
	env_str[key_len] = '=';
	strcpy(env_str + key_len + 1, value);
	return (env_str);
}

int	env_resize(t_env *env)
{
	char	**new_array;
	int		new_size;
	int		i;

	if (env->used_size < env->array_size)
		return (0);
	new_size = env->array_size * 2;
	new_array = malloc(sizeof(char *) * (new_size + 1));
	if (!new_array)
		return (-1);
	i = 0;
	while (i < env->used_size)
	{
		new_array[i] = env->env_array[i];
		i++;
	}
	new_array[i] = NULL;
	free(env->env_array);
	env->env_array = new_array;
	env->array_size = new_size;
	return (0);
}

char	**env_get_array(t_env *env)
{
	return (env->env_array);
}
