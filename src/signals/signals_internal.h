/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 18:31:55 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/06 19:16:22 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_INTERNAL_H
# define SIGNALS_INTERNAL_H

# include <signal.h>

extern volatile sig_atomic_t	g_signals_caught_signum;

void	signals_handler_normal(int signum);
void	signals_handler_input(int signum);

#endif
