/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:50:10 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/02 17:55:36 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "ast.h"
# include "tokenizer.h"

enum	e_parser_status
{
	PARSER_SUCCESS,
	PARSER_ERR_SYNTAX,
	PARSER_ERR_MALLOC,
};

enum e_parser_status	parser_parse(struct s_ast_list_entry **root);

#endif
