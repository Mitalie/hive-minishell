/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:27:10 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/08 20:28:36 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word.h"
#include "word_internal.h"

#include <stdlib.h>

static struct s_word_field	*word_expand_expsplit(char *word)
{
	char				*expanded;
	struct s_word_field	*splits;

	expanded = word_vars(word);
	free(word);
	splits = word_split(expanded);
	free(expanded);
	return (splits);
}

struct s_word_field	*word_expand(char *word)
{
	struct s_word_field	*splits;
	struct s_word_field	*first_field;
	struct s_word_field	**append;
	struct s_word_field	*filenames;

	first_field = NULL;
	append = &first_field;
	splits = word_expand_expsplit(word);
	while (splits)
	{
		filenames = word_filename(splits->value);
		if (!filenames)
		{
			word_unquote(splits->value);
			*append = splits;
			append = &splits->next;
			continue ;
		}
		*append = filenames;
		while (filenames->next)
			filenames = filenames->next;
		append = &filenames->next;
	}
}
