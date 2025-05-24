
#include "../minishell.h"


void print_token(t_token *token)
{
    if (token == NULL)
        return;
    if (token->type == TOKEN_WORD)
        printf("WORD: %s\n", token->value);
    else if (token->type == TOKEN_PIPE)
        printf("PIPE: %s\n", token->value);
    else if (token->type == TOKEN_APPEND)
        printf("APPEND: %s\n", token->value);
    else if (token->type == TOKEN_HEREDOC)
        printf("HEREDOC: %s\n", token->value);
    else if (token->type == TOKEN_REDIRECT_IN)
        printf("REDIRECT_IN: %s\n", token->value);
    else if (token->type == TOKEN_REDIRECT_OUT)
        printf("REDIRECT_OUT: %s\n", token->value);
}

void print_tokens(t_token *tokens)
{
    t_token *current = tokens;

    while (current)
    {
        print_token(current);
        current = current->next;
    }
}

void   print_cmd(t_cmd *cmd)
{
    t_cmd *current = cmd;
    while (current)
    {
        if (current->line)
            printf("line: %s\n", current->line);
        if (current->argv)
        {
            printf("argv: \n");
            for (int i = 0; current->argv[i]; i++)
                printf("\t\t%s \n", current->argv[i]);
        }
        if (current->files)
        {
            printf("files: ");
            for (int i = 0; current->files[i].filename; i++)
                printf("filename: %s\t |type: %u\t|fd: %d\n", current->files[i].filename, current->files[i].type, current->files[i].fd);
        }
        printf("---------------------\n");
        current = current->next;
    }
}