/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 21:30:33 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/12 16:50:57 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_INTERNAL_H
# define EXECUTE_INTERNAL_H

# include "execute.h"
# include "ast.h"
# include <unistd.h>

/*
	Internal functions for command execution
	These functions handle the details of executing commands and redirections
*/
void	handle_absolute_path(char **argv, t_env *env, int *exit_code);

void	handle_path_search(char **argv, t_env *env, int *exit_code);

#endif
