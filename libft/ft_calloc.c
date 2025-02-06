/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 12:23:22 by amakinen          #+#    #+#             */
/*   Updated: 2024/05/02 11:08:07 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <stdint.h>

/*
	C allows implementations to decide whether to return a NULL pointer or a
	unique freeable pointer for zero size allocations. The libft assignment
	requires matching the libc function behaviour, so we avoid filtering zero
	sizes with an early return and pass them on to libc malloc.

	Moulinette seems to require non-NULL pointer, so this implementation would
	fail if malloc returned NULL. That doesn't happen on our platform, and it's
	not clear what would be expected on platforms where it doesn't.
*/

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	if (size && count > SIZE_MAX / size)
		return (0);
	ptr = malloc(count * size);
	if (ptr)
		ft_bzero(ptr, count * size);
	return (ptr);
}
