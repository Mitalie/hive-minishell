/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 11:18:44 by amakinen          #+#    #+#             */
/*   Updated: 2024/04/25 15:42:04 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	int	cmp;

	cmp = 0;
	while (cmp == 0 && n-- && (*s1 || *s2))
		cmp = (unsigned char)*s1++ - (unsigned char)*s2++;
	return (cmp);
}
