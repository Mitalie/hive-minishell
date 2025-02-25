/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_internal.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:27:27 by amakinen          #+#    #+#             */
/*   Updated: 2025/02/25 17:30:11 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPELINE_INTERNAL_H
# define PIPELINE_INTERNAL_H

struct	s_pipeline_fds
{
	int	in;
	int	next_in;
	int	out;
};

# define NO_PIPE -1

#endif
