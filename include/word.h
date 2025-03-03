/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:50:15 by amakinen          #+#    #+#             */
/*   Updated: 2025/02/28 17:15:29 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORD_H
# define WORD_H

/*
	Field resulting from word expansion, allocated as a single block with enough
	space for a next pointer and the string.
*/
struct s_word_field
{
	struct s_word_field	*next;
	char				value[];
};

struct s_word_field	*word_expand(char *word);

#endif
