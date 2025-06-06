/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 18:18:07 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/06 19:38:45 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <stdbool.h>

void	signals_set_normal(void);
void	signals_set_input(void);
void	signals_reset(void);
bool	signals_check_caught(void);

#endif
