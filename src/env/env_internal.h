/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_internal.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 03:25:51 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/01 21:20:35 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_INTERNAL_H
# define ENV_INTERNAL_H

# include "env.h"
# include "status.h"

# include <stdbool.h>
# include <stddef.h>

int			count_array_size(char **array);
int			env_find_index(t_env *env, const char *key, size_t key_len);
char		*create_env_string(const char *key, size_t key_len,
				const char *value, size_t value_len);
t_status	env_resize(t_env *env);

#endif