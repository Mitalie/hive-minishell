/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 17:16:12 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/18 18:56:37 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

#include <stdlib.h>

void	input_init(t_input *input)
{
	input->multiline = NULL;
}

void	input_free(t_input *input)
{
	if (input->multiline)
		free(input->multiline);
}
