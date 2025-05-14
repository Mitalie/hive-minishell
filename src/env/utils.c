/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 01:54:36 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/14 18:10:05 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "env_internal.h"

#include <stdbool.h>
#include <stdlib.h>

#include "libft.h"

bool	env_find_index(t_env *env, const char *key,
	size_t key_len, size_t *idx_out)
{
	size_t	i;

	i = 0;
	while (i < env->used_size)
	{
		if (ft_strncmp(env->env_array[i], key, key_len) == 0
			&& env->env_array[i][key_len] == '=')
		{
			*idx_out = i;
			return (true);
		}
		i++;
	}
	return (false);
}

/*
	Allocate a string for an environment value. Due to limit of four arguments,
	returns a pointer (NULL on allocation failure) instead of t_status. Caller
	must check the pointer and report allocation error if it is NULL.
*/
char	*create_env_string(const char *key, size_t key_len,
		const char *value, size_t value_len)
{
	char	*env_str;

	env_str = malloc(key_len + value_len + 2);
	if (!env_str)
		return (NULL);
	ft_strlcpy(env_str, key, key_len + 1);
	env_str[key_len] = '=';
	ft_strlcpy(env_str + key_len + 1, value, value_len + 1);
	return (env_str);
}

t_status	env_resize(t_env *env)
{
	char	**new_array;
	int		new_size;

	if (env->used_size < env->array_size)
		return (S_OK);
	new_size = env->array_size * 2;
	new_array = malloc(sizeof(char *) * (new_size + 1));
	if (!new_array)
		return (status_err(S_EXIT_ERR, ERRMSG_MALLOC, NULL, 0));
	ft_memcpy(new_array, env->env_array, sizeof(char *) * env->used_size);
	new_array[env->used_size] = NULL;
	free(env->env_array);
	env->env_array = new_array;
	env->array_size = new_size;
	return (S_OK);
}

char	**env_get_array(t_env *env)
{
	return (env->env_array);
}
