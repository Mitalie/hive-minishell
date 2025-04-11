/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_filename.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:48:35 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/11 16:46:53 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word_internal.h"

#include <stdbool.h>

/*
	Match a pattern against files in the current directory and append matching
	file names as fields to the given append pointer.

	TODO: implement
*/
void	word_filename(char	*pattern, struct s_word_field ***matches_append,
	bool *had_matches)
{
	(void)pattern;
	(void)matches_append;
	*had_matches = false;
}
