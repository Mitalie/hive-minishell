/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordexp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:56:49 by amakinen          #+#    #+#             */
/*   Updated: 2025/05/26 18:42:19 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "status.h"
#include "word.h"

int	main(int argc, char **argv)
{
	int					i;
	struct s_word_field	*fields;
	struct s_word_field	**fields_append;
	struct s_word_field	*next;
	t_status			status;

	i = 1;
	while (i < argc)
	{
		printf("\e[31mWord  : \e[91m%s\e[0m\n", argv[i]);
		fields_append = &fields;
		status = word_expand(argv[i], &fields_append);
		while (fields)
		{
			if (status == S_OK)
				printf("\e[32mField : \e[92m%s\e[0m\n", fields->value);
			next = fields->next;
			free(fields);
			fields = next;
		}
		if (status != S_OK)
			return (1);
		i++;
	}
}
