/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_internal.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 16:30:59 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/08 18:50:14 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORD_INTERNAL_H
# define WORD_INTERNAL_H

# include <stddef.h>

/*
	Quote characters originating from expansions are suppressed using an escape
	character. Any instances of this character in the word or expansion values
	must also be escaped.
*/
# define INTERNAL_ESCAPE '\001'

struct	s_word_vars_state
{
	char	*word;
	char	*out;
	size_t	out_idx;
};

struct	s_word_split_state
{
	char				*word;
	struct s_word_field	**out_append;
	struct s_word_field	*out;
	size_t				out_idx;
};

char				*word_vars(char *word);
struct s_word_field	*word_split(char *word);
struct s_word_field	*word_filename(char *pattern);
void				word_unquote(char *str);

#endif
