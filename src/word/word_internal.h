/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_internal.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:24:21 by amakinen          #+#    #+#             */
/*   Updated: 2025/05/09 18:17:44 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORD_INTERNAL_H
# define WORD_INTERNAL_H

# include <stdbool.h>
# include <stddef.h>

# include "status.h"

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

struct	s_word_pattern
{
	char	*pattern;
	size_t	min_len;
	char	*prefix;
	size_t	prefix_len;
};

char		*word_exp_parse(char **word);

t_status	word_filename(char *pattern, struct s_word_field ***matches_append,
				bool *had_matches);

void		word_out_char(struct s_word_state *state, char c, bool quoted);
t_status	word_out_split(struct s_word_state *state);

bool		word_pattern_init_filename(
				struct s_word_pattern *pattern, char *str);
bool		word_pattern_test_filename(
				struct s_word_pattern *pattern, char *str);

t_status	word_scan(struct s_word_state *state);

void		word_unescape(char *escaped);

#endif
