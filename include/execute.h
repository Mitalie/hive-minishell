/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:28:43 by amakinen          #+#    #+#             */
/*   Updated: 2025/02/25 17:48:26 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

struct	s_ast_simple_command;

void	execute_simple_command(struct s_ast_simple_command *command);
void	execute_pipeline(struct s_ast_simple_command *pipeline_head);

#endif
