/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax_error.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:22:58 by josmanov          #+#    #+#             */
/*   Updated: 2025/05/12 22:44:01 by amakinen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_internal.h"

#include "status.h"

/*
	Report a syntax error.

	TODO: print expected and actual tokens, or position in input
*/
t_status	parser_syntax_error(const char *message)
{
	return (status_err(S_RESET_SYNTAX, "syntax error", message, 0));
}
