/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shenv_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 03:25:51 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/04 20:17:47 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHENV_INTERNAL_H
# define SHENV_INTERNAL_H

# include "shenv.h"

# include <stddef.h>
# include <stdbool.h>

# include "status.h"

bool		shenv_var_find_index(t_shenv *env, const char *key,
				size_t key_len, size_t *idx_out);
char		*shenv_var_create_entry(const char *key, size_t key_len,
				const char *value, size_t value_len);
t_status	shenv_var_array_resize(t_shenv *env);

#endif
