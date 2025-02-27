/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:16:04 by josmanov          #+#    #+#             */
/*   Updated: 2025/02/27 19:17:50 by josmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/* TO DO: switch and use libft functions*/
/* TO DO: CODE WAY  TO LONG. NEEDS TO BE REFACTOREDDD */
/* fixed with strdup: The program crashes when freeing string literals  */

#include "parser.h"
#include "ast.h"
#include <stdlib.h>

/*
	 Parses a simple command from the token list.
	 Constructs an AST node for the command, including arguments and redirections.
	 Returns the created command node or NULL on failure.
*/

/* CODE WAY  TO LONG. NEEDS TO BE REFACTOREDDD */
struct s_ast_simple_command *parse_simple_command(struct s_token **tokens)
{
    struct s_ast_simple_command *cmd;
    struct s_ast_command_word *arg;
    struct s_ast_redirect *redir;
    enum e_ast_redirect_op op;

    if (!tokens || !(*tokens))
        return (NULL);

    cmd = malloc(sizeof(struct s_ast_simple_command));
    if (!cmd)
        return (NULL);
    cmd->args = NULL;
    cmd->redirs = NULL;
    cmd->next = NULL;

    while (*tokens && (*tokens)->type != TOK_PIPE && (*tokens)->type != TOK_AND && (*tokens)->type != TOK_OR && (*tokens)->type != TOK_END)
    {
        if ((*tokens)->type == TOK_WORD)
        {
            arg = create_command_word((*tokens)->word_content);
            if (!arg)
                return (NULL);
            arg->next = cmd->args;
            cmd->args = arg;
        }
        else if ((*tokens)->type >= TOK_REDIR_IN && (*tokens)->type <= TOK_HEREDOC)
        {
            if ((*tokens)->type == TOK_REDIR_IN)
                op = AST_REDIR_IN;
            else if ((*tokens)->type == TOK_REDIR_OUT)
                op = AST_REDIR_OUT;
            else if ((*tokens)->type == TOK_REDIR_APP)
                op = AST_REDIR_APP;
            else
                op = AST_HEREDOC;

            (*tokens)++;
            if (!(*tokens) || (*tokens)->type != TOK_WORD)
                return NULL;

            redir = create_redirect(op, (*tokens)->word_content);
            if (!redir)
                return (NULL);
            redir->next = cmd->redirs;
            cmd->redirs = redir;
        }
        else
        {
            break;
        }
        (*tokens)++;
    }
    return (cmd);
}
