/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 11:27:22 by amakinen          #+#    #+#             */
/*   Updated: 2024/04/25 16:52:55 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

static int	utoa_arr(unsigned int n, char *arr)
{
	int	idx;

	idx = 0;
	if (n >= 10)
		idx += utoa_arr(n / 10, arr);
	arr[idx] = '0' + n % 10;
	return (idx + 1);
}

void	ft_putnbr_fd(int n, int fd)
{
	char	arr[11];
	int		idx;

	if (n < 0)
	{
		arr[0] = '-';
		idx = 1 + utoa_arr(-(unsigned int)n, arr + 1);
	}
	else
		idx = utoa_arr(n, arr);
	write(fd, arr, idx);
}
