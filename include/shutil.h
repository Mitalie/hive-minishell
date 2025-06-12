/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shutil.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 21:17:44 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/11 21:02:00 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHUTIL_H
# define SHUTIL_H

# include "shenv.h"

void	shutil_process_wstatus(int wstatus, t_shenv *env);

#endif
