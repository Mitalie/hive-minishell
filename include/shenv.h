/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shenv.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 13:24:48 by josmanov          #+#    #+#             */
/*   Updated: 2025/06/04 20:38:00 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHENV_H
# define SHENV_H

# include <stddef.h>

# include "status.h"

/* Environment structure */
typedef struct s_shenv
{
	char	**var_array;
	size_t	var_array_size;
	size_t	var_array_used;
	int		exit_code;
}	t_shenv;

/* Initialize environment from extern environ */
t_status	shenv_init(t_shenv *env);

/* Free all allocated environment memory */
void		shenv_free(t_shenv *env);

/* Get environment variable value */
char		*shenv_var_get(t_shenv *env, const char *key);

/* Set environment variable */
t_status	shenv_var_set(t_shenv *env, const char *key, const char *value);

/* Unset environment variable */
t_status	shenv_var_unset(t_shenv *env, const char *key);

#endif
