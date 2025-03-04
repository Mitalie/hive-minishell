/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordexp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:56:49 by amakinen          #+#    #+#             */
/*   Updated: 2025/03/04 18:47:55 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "word.h"

int	main(int argc, char **argv)
{
	int					i;
	struct s_word_field	*fields;
	struct s_word_field	*next;

	i = 1;
	while (i < argc)
	{
		printf("\e[31mWord  : \e[91m%s\e[0m\n", argv[i]);
		fields = word_expand(argv[i]);
		while (fields)
		{
			printf("\e[32mField : \e[92m%s\e[0m\n", fields->value);
			next = fields->next;
			free(fields);
			fields = next;
		}
		i++;
	}
}
