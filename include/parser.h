/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:50:10 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/10 22:23:52 by josmanov         ###   ########.fr       */
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
