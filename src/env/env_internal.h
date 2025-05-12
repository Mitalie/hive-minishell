/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_internal.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 03:25:51 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/12 19:29:03 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_INTERNAL_H
# define ENV_INTERNAL_H

# include "env.h"

# include <stddef.h>
# include <stdbool.h>

# include "status.h"

bool		env_find_index(t_env *env, const char *key,
				size_t key_len, size_t *idx_out);
char		*create_env_string(const char *key, size_t key_len,
				const char *value, size_t value_len);
t_status	env_resize(t_env *env);

#endif
