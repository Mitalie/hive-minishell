/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:23:45 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/18 15:40:41 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_INTERNAL_H
# define INPUT_INTERNAL_H

# include "status.h"

t_status	input_notty_get_line(char **line_out, const char *prompt);
t_status	input_tty_get_line(char **line_out, const char *prompt);
void		input_tty_add_history(const char *line);
void		input_tty_clear_history(void);

#endif
