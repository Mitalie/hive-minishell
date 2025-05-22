/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:50:15 by amakinen          #+#    #+#             */
/*   Updated: 2025/05/22 14:11:01 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORD_H
# define WORD_H

# include <stdbool.h>
# include <stddef.h>

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

t_status	word_expand(char *word,
				struct s_word_field ***fields_append, size_t *num_fields);

bool		word_heredoc_delimiter(char *word);
t_status	word_heredoc_line(char **line);

#endif
