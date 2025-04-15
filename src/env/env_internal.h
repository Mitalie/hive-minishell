/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_internal.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 03:25:51 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/13 14:22:21 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_INTERNAL_H
# define ENV_INTERNAL_H

# include "env.h"

int		count_array_size(char **array);
int		env_find_index(t_env *env, const char *key);
char	*create_env_string(const char *key, const char *value);
int		env_resize(t_env *env);

#endif