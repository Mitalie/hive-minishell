/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:28:43 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/21 18:46:39 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "env.h"

struct	s_ast_simple_command;
struct	s_ast_list_entry;

void	execute_simple_command(struct s_ast_simple_command *command,
			t_env *env);
int		execute_pipeline(struct s_ast_simple_command *pipeline_head,
			t_env *env);
int		execute_list(struct s_ast_list_entry *list_head,
			t_env *env);

#endif
