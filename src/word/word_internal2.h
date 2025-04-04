/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_internal2.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:53:03 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/04 17:28:03 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORD_INTERNAL2_H
# define WORD_INTERNAL2_H

# include <stdbool.h>
# include <stddef.h>

# include "word.h"

struct	s_word_expand_scan
{
	char	*word;
	char	*var;
	char	quote;
};

struct s_word_expand_state
{
	struct s_word_expand_scan	count;
	struct s_word_expand_scan	write;
	bool						is_write_scan;
	bool						has_filename_wildcard;
	size_t						out_idx;
	struct s_word_field			*out_curr;
	struct s_word_field			**out_append;
};

#endif
