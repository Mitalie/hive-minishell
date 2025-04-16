/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_internal.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:24:21 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/16 15:08:06 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORD_INTERNAL_H
# define WORD_INTERNAL_H

# include <stdbool.h>
# include <stddef.h>

# define INTERNAL_ESCAPE '\001'

struct	s_word_field;

struct	s_word_state
{
	char				*word;
	struct s_word_field	*out;
	struct s_word_field	**out_append;
	size_t				out_idx;
	bool				out_has_quotes;
	bool				out_has_escape;
	bool				out_has_wildcard;
};

char	*word_exp_parse(struct s_word_state *state);

void	word_filename(char	*pattern, struct s_word_field ***matches_append,
			bool *had_matches);

void	word_out_char(struct s_word_state *state, char c, bool quoted);
void	word_out_split(struct s_word_state *state);

void	word_scan(struct s_word_state *state);

void	word_unescape(char *escaped);

#endif
