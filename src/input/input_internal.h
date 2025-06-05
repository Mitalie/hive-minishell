/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:39:14 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/05 22:10:35 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_INTERNAL_H
# define INPUT_INTERNAL_H

# include "input.h"

/*
	Linked list for storing all the lines going into a single command, including
	heredoc lines, so that they can be combined into a single history entry.

	`owned` is set by default and then cleared on lines that are stored in the
	AST and shouldn't be freed by the input module.
*/
struct s_linelist
{
	struct s_linelist	*next;
	char				*line;
	bool				owned;
};

struct s_input
{
	char				*input_buf;
	// char				*input_pos;
	bool				input_eof;
	// struct s_linelist	*current_lines;
	// struct s_linelist	**current_lines_append;
};

#endif
