/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 13:24:48 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/01 22:20:20 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include <stdbool.h>
# include "status.h"

/* Environment metadata structure */
typedef struct s_env_meta
{
	int		array_size;
	int		used_size;
}	t_env_meta;

/* Environment structure */
typedef struct s_env
{
	char		**env_array;
	t_env_meta	meta;
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

/* Get environment array for execve */
char		**env_get_array(t_env *env);

#endif