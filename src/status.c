/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 20:11:15 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/27 21:30:00 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "status.h"

#include <string.h>
#include <unistd.h>

#include "libft.h"
#include "util.h"

t_status	status_err(t_status status,
	const char *msg, const char *extra, int errnum)
{
	status_warn(msg, extra, errnum);
	return (status);
}

/*
	We want to avoid using malloc as this might be called after malloc failure.
	We should make the local buffer large enough for all the error messages we
	might print, but with strlcpy we'll at least be safe from buffer overflows.
*/
#define ERR_BUF_LEN 1000

void	status_warn(const char *msg, const char *extra, int errnum)
{
	char	buf[ERR_BUF_LEN];
	size_t	len;

	len = ft_strlcpy(buf, msg, ERR_BUF_LEN);
	if (extra)
	{
		len += ft_strlcpy(buf + len, ": ", ERR_BUF_LEN - len);
		len += ft_strlcpy(buf + len, extra, ERR_BUF_LEN - len);
	}
	if (errnum != 0)
	{
		len += ft_strlcpy(buf + len, ": ", ERR_BUF_LEN - len);
		len += ft_strlcpy(buf + len, strerror(errnum), ERR_BUF_LEN - len);
	}
	util_write_all(STDERR_FILENO, buf, len);
}
