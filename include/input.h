/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:39:08 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/05 20:30:27 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include <stdbool.h>
# include <stddef.h>

# include "status.h"

typedef struct s_input	t_input;

t_status	input_init(t_input **input_out);
void		input_free(t_input *input);

/*
	Attempt to get a line of input from the input module. This function blocks
	until a line of input is available. If `SIGINT` is received while waiting,
	still waits for a line of input to become available, but then returns
	`S_RESET_SIGINT` and stores the line to be returned on the next call.
*/
t_status	input_get_line(t_input *input, char **line_out);

/*
	Inform the input module that the parsing of a command has ended. If
	`store_history` is true the lines of this command are saved as a single
	history entry. Otherwise the lines of this command are discarded.
*/
t_status	input_finish_command(t_input *input, bool store_history);

/*
	Take ownership of the last line returned from `input_get_line`. Input module
	will not free this line. Caller takes responsibility for freeing the line,
	but must not do so before calling `input_finish_command`.
*/
void		input_keep_last_line(t_input *input);

#endif
