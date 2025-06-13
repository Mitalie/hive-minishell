/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shenv_var_get_set.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 02:15:43 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/13 16:58:07 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shenv.h"
#include "shenv_internal.h"

#include <stdlib.h>

#include "libft.h"
#include "status.h"

char	*shenv_var_get(t_shenv *env, const char *key)
{
	size_t	index;
	size_t	key_len;

	key_len = ft_strlen(key);
	if (!shenv_var_find_index(env, key, key_len, &index))
		return (NULL);
	return (env->var_array[index] + key_len + 1);
}

static t_status	shenv_var_store_entry(t_shenv *env, char *new_entry,
	size_t key_len)
{
	t_status	status;
	size_t		index;

	if (shenv_var_find_index(env, new_entry, key_len, &index))
	{
		free(env->var_array[index]);
		env->var_array[index] = new_entry;
		return (S_OK);
	}
	status = shenv_var_array_resize(env);
	if (status != S_OK)
	{
		free(new_entry);
		return (status);
	}
	env->var_array[env->var_array_used] = new_entry;
	env->var_array_used++;
	env->var_array[env->var_array_used] = NULL;
	return (S_OK);
}

/*
	Allocate a string for an environment value. Due to limit of four arguments,
	returns a pointer (NULL on allocation failure) instead of t_status. Caller
	must check the pointer and report allocation error if it is NULL.
*/
static char	*shenv_var_create_entry(const char *key, size_t key_len,
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

t_status	shenv_var_set(t_shenv *env, const char *key, const char *value)
{
	char	*new_entry;
	size_t	key_len;
	size_t	value_len;

	key_len = ft_strlen(key);
	value_len = ft_strlen(value);
	new_entry = shenv_var_create_entry(key, key_len, value, value_len);
	if (!new_entry)
		return (status_err(S_EXIT_ERR, ERRMSG_MALLOC, NULL, 0));
	return (shenv_var_store_entry(env, new_entry, key_len));
}

t_status	shenv_var_unset(t_shenv *env, const char *key)
{
	size_t	index;
	size_t	key_len;

	key_len = ft_strlen(key);
	if (!shenv_var_find_index(env, key, key_len, &index))
		return (S_OK);
	free(env->var_array[index]);
	ft_memmove(&env->var_array[index], &env->var_array[index + 1],
		sizeof(char *) * (env->var_array_used - index));
	env->var_array_used--;
	env->var_array[env->var_array_used] = NULL;
	return (S_OK);
}
