/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:28:43 by amakinen          #+#    #+#             */
/*   Updated: 2025/05/29 20:10:05 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include <stdbool.h>

# include "env.h"
# include "status.h"

struct	s_ast_simple_command;
struct	s_ast_list_entry;
struct	s_ast_redirect;

t_status	execute_simple_command(struct s_ast_simple_command *command,
				t_env *env, int *exit_code, bool is_child);
t_status	execute_pipeline(struct s_ast_simple_command *pipeline_head,
				t_env *env, int *exit_code);
t_status	execute_list(struct s_ast_list_entry *list_head,
				t_env *env, int *exit_code);
int			process_heredoc(struct s_ast_redirect *redirect);

#endif
