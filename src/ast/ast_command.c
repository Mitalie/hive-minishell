/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 12:21:43 by josmanov          #+#    #+#             */
/*   Updated: 2025/03/23 14:31:51 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include <stdlib.h>

/*
	Creates a command word node for the AST.
	Allocates memory for the node and assigns the input word directly.
	Returns the created node or NULL on failure.
*/
struct s_ast_command_word	*create_command_word(char *word)
{
	struct s_ast_command_word	*node;

	node = malloc(sizeof(struct s_ast_command_word));
	if (!node)
		return (NULL);
	node->word = word;
	node->next = NULL;
	return (node);
}
