/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 21:26:43 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/05 22:10:51 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include "input_internal.h"

#include <stdlib.h>

#include "status.h"

	// input->input_pos = NULL;
	// input->current_lines = NULL;
	// input->current_lines_append = &input->current_lines;

t_status	input_init(t_input **input_out)
{
	t_input	*input;

	input = malloc(sizeof(*input));
	*input_out = input;
	if (!input)
		return (status_err(S_EXIT_ERR, ERRMSG_MALLOC, NULL, 0));
	input->input_buf = NULL;
	input->input_eof = false;
	return (S_OK);
}

// static void	input_free_linelist(struct s_linelist *list)
// {
// 	struct s_linelist	*next;

// 	while (list)
// 	{
// 		next = list->next;
// 		if (list->owned)
// 			free(list->line);
// 		free(list);
// 		list = next;
// 	}
// }

	// input_free_linelist(input->current_lines);

void	input_free(t_input *input)
{
	if (!input)
		return ;
	free(input->input_buf);
	free(input);
}
