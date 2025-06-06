/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:27:27 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/04 23:27:47 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_INTERNAL_H
# define EXECUTE_INTERNAL_H

# include <stdbool.h>

# include "ast.h"
# include "shenv.h"
# include "status.h"

/*
	Internal functions for command execution
	These functions handle the finding and executing external commands
*/
void		handle_absolute_path(char **argv, t_shenv *env);

void		handle_path_search(char **argv, t_shenv *env);

struct	s_pipeline_fds
{
	int	in;
	int	next_in;
	int	out;
};

# define NO_PIPE -1

t_status	execute_pipeline(struct s_ast_simple_command *pipeline_head,
				t_shenv *env);

t_status	execute_simple_command(struct s_ast_simple_command *command,
				t_shenv *env, bool is_child);

struct s_redir_fds
{
	int	in;
	int	out;
};

# define NO_REDIR -1

t_status	execute_redirect_heredoc(struct s_ast_redirect *redirect,
				struct s_redir_fds *fds, t_shenv *env);

t_status	execute_redirect_prepare(struct s_redir_fds *fds,
				struct s_ast_redirect *redirs, t_shenv *env);
t_status	execute_redirect_finish(struct s_redir_fds *fds, bool apply);

#endif
