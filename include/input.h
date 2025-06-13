/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 17:45:47 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/13 18:50:19 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include "status.h"

t_status	input_get_line(char **line_out, const char *prompt);

void		input_add_history(const char *line);

void		input_clear_history(void);

t_status	input_notty_get_line(char **line_out, const char *prompt);

#endif
