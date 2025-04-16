/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 13:56:32 by amakinen          #+#    #+#             */
/*   Updated: 2025/04/16 16:37:32 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "word_internal.h"

#include <stdbool.h>
#include <stdlib.h>

static size_t	word_heredoc_line_scan(char *scan, char *out, bool *had_exp);

/*
	Process heredoc delimiter by removing quotes that are not themselves quoted.
	The word is modified in-place. Returns true iff any quotes were removed.
*/
bool	word_heredoc_delimiter(char *word)
{
	char	quote;
	char	c;
	char	*unquoted;

	unquoted = word;
	quote = 0;
	while (*word)
	{
		c = *word++;
		if (c == quote)
			quote = 0;
		else if ((c == '\'' || c == '"') && !quote)
			quote = c;
		else
			*unquoted++ = c;
	}
	*unquoted = '\0';
	return (word != unquoted);
}

/*
	Process parameter expansions on a heredoc line. If any expansions are done,
	allocates a new string and frees the original. In case of malloc failure, a
	null pointer is returned and the original string is freed.
*/
char	*word_heredoc_line(char *line)
{
	char	*out;
	size_t	out_len;
	bool	had_exp;

	had_exp = false;
	out_len = word_heredoc_line_scan(line, NULL, &had_exp);
	if (!had_exp)
		return (line);
	out = malloc(out_len + 1);
	if (out)
		word_heredoc_line_scan(line, out, &had_exp);
	if (out)
		out[out_len] = '\0';
	free(line);
	return (out);
}

/*
	Scan the input for parameter expansions and count the size of the output.
	If out is not null, copy characters to output. Set had_exp to true if an
	expansion is found.
*/
static size_t	word_heredoc_line_scan(char *scan, char *out, bool *had_exp)
{
	char	c;
	char	*value;
	size_t	out_idx;

	out_idx = 0;
	while (*scan)
	{
		c = *scan++;
		if (c == '$')
			value = word_exp_parse(&scan);
		if (c == '$' && value)
		{
			while (out && *value)
				out[out_idx++] = *value++;
			while (!out && *value++)
				out_idx++;
			*had_exp = true;
			continue ;
		}
		if (out)
			out[out_idx] = c;
		out_idx++;
	}
	return (out_idx);
}
