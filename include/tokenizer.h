/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:50:25 by amakinen          #+#    #+#             */
/*   Updated: 2025/02/19 18:21:09 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

enum	e_token
{
	TOK_WORD,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_REDIR_APP,
	TOK_HEREDOC,
	TOK_PIPE,
	TOK_AND,
	TOK_OR,
	TOK_GROUP_START,
	TOK_GROUP_END,
	TOK_END,
};

typedef struct s_token
{
	enum e_token	type;
	char			*word_content;
}	t_token;

typedef struct s_tokenizer_state
{
	char	*line;
	char	*line_pos;
}	t_tokenizer_state;

typedef struct s_operator_def
{
	const char		*str;
	enum e_token	type;
}	t_operator_def;

t_token	tokenizer_get_next(t_tokenizer_state *state);

#endif
