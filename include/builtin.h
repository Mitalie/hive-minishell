/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 15:12:18 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/04 20:14:39 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "shenv.h"
# include "status.h"

typedef t_status	t_builtin_func(char	**argv, t_shenv *env,
						int *exit_code, int stdout_fd);

t_builtin_func	*builtin_get_func(const char *name);

#endif
