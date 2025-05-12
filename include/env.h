/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 13:24:48 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/12 19:09:18 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include <stddef.h>

# include "status.h"

/* Environment structure */
typedef struct s_env
{
	char	**env_array;
	size_t	array_size;
	size_t	used_size;
}	t_env;

/* Initialize environment from extern environ */
t_status	env_init(t_env *env);

/* Free all allocated environment memory */
void		env_free(t_env *env);

/* Get environment variable value */
char		*env_get(t_env *env, const char *key);

/* Set environment variable */
t_status	env_set(t_env *env, const char *key, const char *value);

/* Unset environment variable */
t_status	env_unset(t_env *env, const char *key);

#endif
