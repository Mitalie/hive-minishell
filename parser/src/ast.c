/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:13:26 by josmanov          #+#    #+#             */
/*   Updated: 2025/02/27 19:20:41 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/*to do switch and use libft functions*/

#include "ast.h"
#include <stdlib.h>
#include <string.h>

/*
	 Creates a command word node for the AST.
	 Allocates memory for the node and duplicates the input word.
	 Returns the created node or NULL on failure.
*/
struct s_ast_command_word *create_command_word(char *word)
{
    struct s_ast_command_word *node;

    node = malloc(sizeof(struct s_ast_command_word));
    if (!node)
        return (NULL);
    node->word = strdup(word);
    if(!node->word){
        free(node);
        return NULL;
    }
    node->next = NULL;
    return (node);
}

/*
	 Creates a redirect node for the AST.
	 Allocates memory for the node and duplicates the input word.
	 Returns the created node or NULL on failure.
*/
struct s_ast_redirect *create_redirect(enum e_ast_redirect_op op, char *word)
{
    struct s_ast_redirect *node;

    node = malloc(sizeof(struct s_ast_redirect));
    if (!node)
        return (NULL);
    node->op = op;
    node->word = strdup(word);
    if(!node->word){
        free(node);
        return NULL;
    }
    node->next = NULL;
    return node;
}
