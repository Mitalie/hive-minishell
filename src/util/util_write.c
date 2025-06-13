/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_write.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 20:28:30 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/26 20:31:28 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>
#include <unistd.h>

/*
	Write entire buffer even if interrupted by a signal handler.
*/
bool	util_write_all(int fd, const void *buf, size_t len)
{
	ssize_t	written;

	while (len)
	{
		written = write(fd, buf, len);
		if (written < 0 && errno == EINTR)
			continue ;
		else if (written < 0)
			return (false);
		buf = (char *)buf + written;
		len -= written;
	}
	return (true);
}
