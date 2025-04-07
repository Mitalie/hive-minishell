/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:50:10 by josmanov          #+#    #+#             */
/*   Updated: 2025/04/07 16:56:10 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "ast.h"

enum	e_parser_status
{
	PARSER_SUCCESS,
	PARSER_ERR_SYNTAX,
	PARSER_ERR_MALLOC,
};

enum e_parser_status	parser_parse(char *line,
							struct s_ast_list_entry **root);

#endif
