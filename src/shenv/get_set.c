/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 02:15:43 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/04 20:17:47 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shenv.h"
#include "shenv_internal.h"

#include <stdbool.h>
#include <stdlib.h>

#include "libft.h"

char	*shenv_var_get(t_shenv *env, const char *key)
{
	size_t	index;
	size_t	key_len;

	key_len = ft_strlen(key);
	if (!shenv_var_find_index(env, key, key_len, &index))
		return (NULL);
	return (env->var_array[index] + key_len + 1);
}

static t_status	update_existing_entry(t_shenv *env, int index, char *new_entry)
{
	free(env->var_array[index]);
	env->var_array[index] = new_entry;
	return (S_OK);
}

static t_status	add_new_entry(t_shenv *env, char *new_entry)
{
	t_status	status;

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

t_status	shenv_var_set(t_shenv *env, const char *key, const char *value)
{
	size_t	index;
	char	*new_entry;
	size_t	key_len;
	size_t	value_len;

	key_len = ft_strlen(key);
	value_len = ft_strlen(value);
	new_entry = shenv_var_create_entry(key, key_len, value, value_len);
	if (!new_entry)
		return (status_err(S_EXIT_ERR, ERRMSG_MALLOC, NULL, 0));
	if (shenv_var_find_index(env, key, key_len, &index))
		return (update_existing_entry(env, index, new_entry));
	return (add_new_entry(env, new_entry));
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
