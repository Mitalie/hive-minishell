/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_redirect.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 19:21:51 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/23 14:31:55 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include <stdlib.h>

/*
	Creates a redirect node for the AST.
	Allocates memory for the node and assigns the input word directly.
	Returns the created node or NULL on failure.
*/
struct s_ast_redirect	*create_redirect(enum e_ast_redirect_op op, char *word)
{
	struct s_ast_redirect	*node;

	node = malloc(sizeof(struct s_ast_redirect));
	if (!node)
		return (NULL);
	node->op = op;
	node->word = word;
	node->next = NULL;
	return (node);
}
