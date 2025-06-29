/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 17:45:47 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/18 18:56:37 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include <stddef.h>

# include "status.h"

typedef struct s_input
{
	char	*multiline;
	size_t	multiline_pos;
}	t_input;

void		input_init(t_input *input);

void		input_free(t_input *input);

t_status	input_get_line(t_input *input, char **line_out, const char *prompt);

void		input_add_history(const char *line);

void		input_clear_history(void);

#endif
