/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 01:54:36 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/21 18:47:27 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "env_internal.h"
#include "libft.h"
#include <stdlib.h>

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

	key_len = ft_strlen(key);
	i = 0;
	while (i < env->meta.used_size)
	{
		if (ft_strncmp(env->env_array[i], key, key_len) == 0
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

	key_len = ft_strlen(key);
	value_len = ft_strlen(value);
	env_str = malloc(key_len + value_len + 2);
	if (!env_str)
		return (NULL);
	ft_strlcpy(env_str, key, key_len + 1);
	env_str[key_len] = '=';
	ft_strlcpy(env_str + key_len + 1, value, value_len + 1);
	return (env_str);
}

int	env_resize(t_env *env)
{
	char	**new_array;
	int		new_size;

	if (env->meta.used_size < env->meta.array_size)
		return (0);
	new_size = env->meta.array_size * 2;
	new_array = malloc(sizeof(char *) * (new_size + 1));
	if (!new_array)
		return (-1);
	ft_memcpy(new_array, env->env_array, sizeof(char *) * env->meta.used_size);
	new_array[env->meta.used_size] = NULL;
	free(env->env_array);
	env->env_array = new_array;
	env->meta.array_size = new_size;
	return (0);
}

char	**env_get_array(t_env *env)
{
	return (env->env_array);
}
