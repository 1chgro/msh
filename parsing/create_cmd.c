#include "../parse.h"


int init_cmd_lst(t_cmd **cmd)
{
    *cmd = malloc(sizeof(t_cmd));
    if (!*cmd)
        return (0);
    (*cmd)->argv = NULL;
    (*cmd)->infile = NULL;
    (*cmd)->outfile = NULL;
    (*cmd)->append = 0;
    // (*cmd)->has_heredoc = 0;
    (*cmd)->heredoc = NULL;
    (*cmd)->next = NULL;
    return (1);
}

int count_words(t_token *tokens)
{
    int count = 0;
    t_token *prev = NULL;

    while (tokens && tokens->type != TOKEN_PIPE)
    {
        if (tokens->type == TOKEN_WORD && !is_redirection(&prev))
            count++;
        prev = tokens;
        tokens = tokens->next;
    }
    return (count);
}

t_cmd *create_cmd_lst(t_token *tokens)
{
    t_cmd *cmd;
    t_cmd *head;
    cmd = NULL;
    t_token *tmp = NULL;
    
    tmp = tokens;
    init_cmd_lst(&cmd);
    head = cmd;
    cmd->argv = malloc(sizeof(char *) * (3));
    cmd->argv[0] = NULL;
    cmd->argv[1] = NULL;
    cmd->argv[2] = NULL;
    while(tokens)
    {
        if (is_redirection(&tokens))
        {   
            if (tokens->type == TOKEN_REDIRECT_OUT || tokens->type == TOKEN_APPEND)
            {
                cmd->outfile = ft_strdup(tokens->next->value);
                if (tokens->type == TOKEN_APPEND)
                    cmd->append = 1;
                tokens = tokens->next;
            }
            if (tokens->type == TOKEN_REDIRECT_IN)
            {
                cmd->infile = ft_strdup(tokens->next->value);
                tokens = tokens->next;
            }
            if (tokens->type == TOKEN_HEREDOC)
            {
                if (cmd->heredoc)
                {
                    t_heredoc *tmp_h = cmd->heredoc;
                    cmd->heredoc = cmd->heredoc->next;
                    cmd->heredoc->prev = tmp_h;
                    cmd->heredoc = malloc(sizeof(t_heredoc));
                    cmd->heredoc->delimiter = ft_strdup(tokens->next->value);
                }
                else if (!cmd->heredoc)
                {
                    cmd->heredoc = malloc(sizeof(t_heredoc));
                    cmd->heredoc->delimiter = ft_strdup(tokens->next->value);
                    cmd->heredoc->prev = NULL;
                    cmd->heredoc->next = NULL;
                }
                tokens = tokens->next;
            }
        }
        if (tokens->type == TOKEN_WORD)
        {
            if (!cmd->argv[0])
                cmd->argv[0] = ft_strdup(tokens->value);
            else if (cmd->argv[0])
                cmd->argv[1] = ft_strjoin(cmd->argv[1], tokens->value);
        }
        if (tokens->type == TOKEN_PIPE)
        {
            init_cmd_lst(&cmd->next);
            cmd = cmd->next;
            cmd->argv =  malloc(sizeof(char *) * (3));
            cmd->argv[0] = NULL;
            cmd->argv[1] = NULL;
            cmd->argv[2] = NULL;
        }
        tokens = tokens->next;
    }
    cmd->next = NULL;
    if (cmd->next)
        DEBUGG_CHECK;
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