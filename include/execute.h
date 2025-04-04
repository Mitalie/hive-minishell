/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:28:43 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/04 20:59:52 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

struct	s_ast_simple_command;
struct	s_ast_list_entry;

void	execute_simple_command(struct s_ast_simple_command *command);
int		execute_pipeline(struct s_ast_simple_command *pipeline_head);
int		execute_list(struct s_ast_list_entry *list_head);

#endif
