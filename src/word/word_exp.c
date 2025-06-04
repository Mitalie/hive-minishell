/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_exp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:12:48 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/04 23:50:35 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word_internal.h"

#include <stdlib.h>

#include "shenv.h"
#include "util.h"

static char	*word_exp_parse_varname(char **word, t_shenv *env)
{
	char		*name_start;
	char		*value;
	char		name_end_tmp;

	name_start = *word;
	while (util_isname(**word))
		(*word)++;
	name_end_tmp = **word;
	**word = '\0';
	value = shenv_var_get(env, name_start);
	**word = name_end_tmp;
	if (!value)
		value = "";
	return (value);
}

/*
	Exit code is converted in a static array. Callers should immediately read
	the string and copy the characters to their output. 12 characters is enough
	for 32-bit INT_MIN (including the minus sign) and the null terminator.
*/
static char	*word_exp_parse_exitcode(char **word, t_shenv *env)
{
	static char	exit_code_str[12];

	(*word)++;
	util_itoa_base(env->exit_code, "0123456789", exit_code_str);
	return (exit_code_str);
}

/*
	Identify and consume the expansion following a `$`, and return a pointer
	to its value. If no valid expansion is found, a null pointer is returned.
*/
char	*word_exp_parse(char **word, t_shenv *env)
{
	if (**word == '?')
		return (word_exp_parse_exitcode(word, env));
	else if (util_isname(**word))
		return (word_exp_parse_varname(word, env));
	else
		return (NULL);
}
