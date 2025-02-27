#include "parser.h"
#include "ast.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>


/*
** Prints the AST structure for a simple command.
** Displays arguments and redirections in a readable format.
*/
void print_ast(struct s_ast_simple_command *cmd)
{
    struct s_ast_command_word *arg;
    struct s_ast_redirect *redir;

    printf("Simple Command:\n");

    printf("  Arguments:\n");
    arg = cmd->args;
    while (arg)
    {
        printf("    %s\n", arg->word);
        arg = arg->next;
    }

    printf("  Redirections:\n");
    redir = cmd->redirs;
    while (redir)
    {
        printf("    ");
        if (redir->op == AST_REDIR_IN)
            printf("< ");
        else if (redir->op == AST_REDIR_OUT)
            printf("> ");
        else if (redir->op == AST_REDIR_APP)
            printf(">> ");
        else if (redir->op == AST_HEREDOC)
            printf("<< ");
        printf("%s\n", redir->word);
        redir = redir->next;
    }
}

int main()
{
    struct s_token tokens[] = {
        {TOK_WORD, "ls"},
        {TOK_WORD, "-l"},
        {TOK_REDIR_OUT, ">"},
        {TOK_WORD, "out.txt"},
        {TOK_END, NULL}};

    struct s_token *token_ptr = tokens;
    struct s_ast_simple_command *cmd = parse_simple_command(&token_ptr);

    if (cmd)
    {
        print_ast(cmd);
    }
    else
    {
        printf("Parsing failed.\n");
    }

    // Freeing the AST (This needs to beee moree comprehensive in aa real scenario)
    struct s_ast_command_word *arg = cmd->args;
    while(arg){
        struct s_ast_command_word *temp = arg;
        arg = arg->next;
        free(temp->word);
        free(temp);
    }

    struct s_ast_redirect *redirect = cmd->redirs;
    while(redirect){
        struct s_ast_redirect *temp = redirect;
        redirect = redirect->next;
        free(temp->word);
        free(temp);
    }
    free(cmd);

    return 0;
}