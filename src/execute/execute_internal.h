/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:27:27 by amakinen          #+#    #+#             */
/*   Updated: 2025/05/12 17:38:26 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_INTERNAL_H
# define EXECUTE_INTERNAL_H

# include "env.h"

/*
	Internal functions for command execution
	These functions handle the finding and executing external commands
*/
void	handle_absolute_path(char **argv, t_env *env, int *exit_code);

void	handle_path_search(char **argv, t_env *env, int *exit_code);

struct	s_pipeline_fds
{
	int	in;
	int	next_in;
	int	out;
};

# define NO_PIPE -1

#endif
