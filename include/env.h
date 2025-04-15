/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 13:24:48 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/15 15:18:31 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

typedef struct s_env
{
	char	**env_array;
	int		array_size;
	int		used_size;
	int		original_size;
}	t_env;

/* Initialize environment from extern environ */
t_env	*env_init(void);

/* Free all allocated environment memory */
void	env_free(t_env *env);

/* Get environment variable value */
char	*env_get(t_env *env, const char *key);

/* Set environment variable */
int		env_set(t_env *env, const char *key, const char *value);

/* Unset environment variable */
int		env_unset(t_env *env, const char *key);

/* Get environment array for execve */
char	**env_get_array(t_env *env);

#endif