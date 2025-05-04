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
    (*cmd)->has_heredoc = 0;
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
    int words;
    
    tmp = tokens;
    init_cmd_lst(&cmd);
    head = cmd;
    words = count_words(tmp);
    cmd->argv = malloc(sizeof(char *) * (words + 1));
    int i = 0;
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
        }
        else if (tokens->type == TOKEN_WORD && i < words)
        {
            cmd->argv[i] = ft_strdup(tokens->value);
            i++;
        }
        if (tokens->type == TOKEN_PIPE)
        {
            cmd->argv[i] = NULL;
            init_cmd_lst(&cmd->next);
            cmd = cmd->next;
            tmp = tokens->next;
            words = count_words(tmp);
            cmd->argv =  malloc(sizeof(char *) * (words + 1));
            i = 0;
        }
        tokens = tokens->next;
    }
    cmd->argv[i] = NULL;
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