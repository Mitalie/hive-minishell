/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:50:15 by amakinen          #+#    #+#             */
/*   Updated: 2025/06/04 21:28:54 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORD_H
# define WORD_H

# include <stdbool.h>

# include "shenv.h"
# include "status.h"

/*
	Field resulting from word expansion, allocated as a single block with enough
	space for a next pointer and the string.
*/
struct s_word_field
{
	struct s_word_field	*next;
	char				value[];
};

void		word_free(struct s_word_field *fields);

t_status	word_expand(char *word, struct s_word_field ***fields_append,
				t_shenv *env);

bool		word_heredoc_delimiter(char *word);
t_status	word_heredoc_line(char **line, t_shenv *env);

#endif
