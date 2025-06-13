/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 11:32:03 by amakinen          #+#    #+#             */
/*   Updated: 2024/04/25 16:52:51 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	utoa_arr(unsigned int n, char *arr)
{
	int	idx;

	idx = 0;
	if (n >= 10)
		idx += utoa_arr(n / 10, arr);
	arr[idx] = '0' + n % 10;
	return (idx + 1);
}

char	*ft_itoa(int n)
{
	char	arr[12];
	int		idx;

	if (n < 0)
	{
		arr[0] = '-';
		idx = 1 + utoa_arr(-(unsigned int)n, arr + 1);
	}
	else
		idx = utoa_arr(n, arr);
	arr[idx] = 0;
	return (ft_strdup(arr));
}
