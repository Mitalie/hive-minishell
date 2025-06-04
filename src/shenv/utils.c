/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 01:54:36 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/04 20:17:47 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shenv.h"
#include "shenv_internal.h"

#include <stdbool.h>
#include <stdlib.h>

#include "libft.h"

bool	shenv_var_find_index(t_shenv *env, const char *key,
	size_t key_len, size_t *idx_out)
{
	size_t	i;

	i = 0;
	while (i < env->var_array_used)
	{
		if (ft_strncmp(env->var_array[i], key, key_len) == 0
			&& (env->var_array[i][key_len] == '='
			|| env->var_array[i][key_len] == '\0'))
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
char	*shenv_var_create_entry(const char *key, size_t key_len,
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

t_status	shenv_var_array_resize(t_shenv *env)
{
	char	**new_array;
	int		new_size;

	if (env->var_array_used < env->var_array_size)
		return (S_OK);
	new_size = env->var_array_size * 2;
	new_array = malloc(sizeof(char *) * (new_size + 1));
	if (!new_array)
		return (status_err(S_EXIT_ERR, ERRMSG_MALLOC, NULL, 0));
	ft_memcpy(new_array, env->var_array, sizeof(char *) * env->var_array_used);
	new_array[env->var_array_used] = NULL;
	free(env->var_array);
	env->var_array = new_array;
	env->var_array_size = new_size;
	return (S_OK);
}
