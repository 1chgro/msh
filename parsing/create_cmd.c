#include "../parse.h"


int init_cmd_lst(t_cmd **cmd)
{
    *cmd = malloc(sizeof(t_cmd));
    if (!*cmd)
        return (0);
    (*cmd)->argv = malloc(sizeof(char *) * 3);
    if (!(*cmd)->argv)
        return (0);
    (*cmd)->argv[0] = NULL;
    (*cmd)->argv[1] = NULL;
    (*cmd)->argv[2] = NULL;
    (*cmd)->infile = NULL;
    (*cmd)->outfile = NULL;
    (*cmd)->append = 0;
    (*cmd)->heredoc = NULL;
    (*cmd)->next = NULL;
    return (1);
}




int count_redirections_in(t_token **tokens)
{
    int count = 0;
    t_token *tmp = *tokens;
    while (tmp && !is_pipe(&tmp))
    {
        if (tmp->type == TOKEN_REDIRECT_IN)
            count++;
        tmp = tmp->next;
    }
    return (count);
}
int count_redirections_out(t_token **tokens)
{
    int count = 0;
    t_token *tmp = *tokens;
    while (tmp && !is_pipe(&tmp))
    {
        if (tmp->type == TOKEN_REDIRECT_OUT || tmp->type == TOKEN_APPEND)
            count++;
        tmp = tmp->next;
    }
    return (count);
}

t_cmd *create_cmd_lst(t_token *tokens)
{
    t_cmd *cmd;
    t_cmd *head;
    cmd = NULL;
    t_token *token_prev = NULL;
    
    int in_redirection_count = 0;
    int out_redirection_count = 0;
    int red_out_pos = 0;
    int red_in_pos = 0;
    in_redirection_count = count_redirections_in(&tokens);
    out_redirection_count = count_redirections_out(&tokens);
    token_prev = tokens;
    init_cmd_lst(&cmd);
    head = cmd;
    while(tokens)
    {
        if (is_redirection(&tokens))
        {
            if (tokens->type ==  TOKEN_REDIRECT_OUT || tokens->type == TOKEN_APPEND)
            {
                if (!cmd->outfile && out_redirection_count > 0)
                {
                    cmd->outfile = malloc(sizeof(char *) * (out_redirection_count + 1));
                    cmd->outfile[0] = NULL;
                }
                cmd->outfile[red_out_pos++] = ft_strdup(tokens->next->value);
                cmd->outfile[red_out_pos] = NULL;
                if (tokens->type == TOKEN_APPEND)
                    cmd->append = 1;
            }
            else if (tokens->type ==  TOKEN_REDIRECT_IN)
            {
                if (!cmd->infile && in_redirection_count > 0)
                {
                    cmd->infile = malloc(sizeof(char *) * (in_redirection_count + 1));
                    cmd->infile[0] = NULL;
                }
                cmd->infile[red_in_pos++] = ft_strdup(tokens->next->value);
                cmd->infile[red_in_pos] = NULL;
            }
            token_prev = tokens;
            tokens = tokens->next;
        }
        else if (tokens->type == TOKEN_WORD && !is_redirection(&token_prev))
        {
            if (!cmd->argv[0])
                cmd->argv[0] = ft_strdup(tokens->value);
            else if (cmd->argv[0])
                cmd->argv[1] = ft_strjoin(cmd->argv[1], tokens->value);
        }
        else if (tokens->type == TOKEN_PIPE)
        {
            init_cmd_lst(&cmd->next);
            in_redirection_count = count_redirections_in(&tokens->next);
            out_redirection_count = count_redirections_out(&tokens->next);
            cmd = cmd->next;
        }
        token_prev = tokens;
        tokens = tokens->next;
    }
    cmd->next = NULL;
    return (head);
}

t_cmd *create_cmd(t_token *tokens)
{
    t_cmd *cmd;

    cmd = NULL;
    if (!tokens)
        return (NULL);
    cmd = create_cmd_lst(tokens);
    return (cmd);
}