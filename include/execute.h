/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:28:43 by amakinen          #+#    #+#             */
/*   Updated: 2025/05/10 22:23:48 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "env.h"

struct	s_ast_simple_command;
struct	s_ast_list_entry;
struct	s_ast_redirect;

void	execute_simple_command(struct s_ast_simple_command *command,
			t_env *env);
int		execute_pipeline(struct s_ast_simple_command *pipeline_head,
			t_env *env);
int		execute_list(struct s_ast_list_entry *list_head,
			t_env *env);
int		process_heredoc(struct s_ast_redirect *redirect);

#endif
