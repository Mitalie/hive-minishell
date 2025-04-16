/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_exp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:12:48 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/16 15:15:01 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word_internal.h"

#include <stdlib.h>

#include "util.h"

/*
	Identify and consume the expansion following a `$`, and return a pointer
	to its value. If no valid expansion is found, a null pointer is returned.

	TODO: implement correct value for `$?`.
*/
char	*word_exp_parse(char **word)
{
	char	*name_start;
	char	*value;
	char	name_end_tmp;

	if (**word == '?')
	{
		(*word)++;
		value = "0";
	}
	else if (util_isname(**word))
	{
		name_start = *word;
		while (util_isname(**word))
			(*word)++;
		name_end_tmp = **word;
		**word = '\0';
		value = getenv(name_start);
		**word = name_end_tmp;
		if (!value)
			value = "";
	}
	else
		return (NULL);
	return (value);
}
