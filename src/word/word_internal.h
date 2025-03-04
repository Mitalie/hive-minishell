/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_internal.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:55:13 by amakinen          #+#    #+#             */
/*   Updated: 2025/03/04 19:34:45 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORD_INTERNAL_H
# define WORD_INTERNAL_H

# include <stdbool.h>
# include <stddef.h>

# include "word.h"

struct	s_word_expand_state
{
	char				*word;
	struct s_word_field	*field_first;
	struct s_word_field	*field_curr;
	size_t				field_pos;
	bool				field_has_unquoted_wildcard;
	bool				write;
};

void	word_unquote(char *str);
void	word_filename(struct s_word_field **field);

#endif
