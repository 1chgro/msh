#include "../parse.h"

void	ft_putstr_fd(char *s, int fd)
{
	size_t	i;

	if (!s)
		return ;
	i = 0;
	while (s[i])
	{
        write(fd, &s[i], 1);
		i++;
	}
}

char *sy_token_type(t_token **token, int n_Line_flg)
{
    if (n_Line_flg == 0)
    {
        if ((*token)->type == TOKEN_PIPE)
        return ("|");
        else if ((*token)->type == TOKEN_APPEND)
            return (">>");
        else if ((*token)->type == TOKEN_HEREDOC)
            return ("<<");
        else if ((*token)->type == TOKEN_REDIRECT_IN)
            return ("<");
        else if ((*token)->type == TOKEN_REDIRECT_OUT)
            return (">");
    }
    return ("newline");
}

void prnt_sy_err(char *value)
{
    ft_putstr_fd("msh: syntax error near unexpected token `", 2);
    ft_putstr_fd(value, 2);
    ft_putstr_fd("'\n", 2);
}


int check_pipe_syntax(t_token *prev, t_token *current)
{
    if (is_pipe(&current))
    {
        if (is_pipe(&prev))
        {
            prnt_sy_err(sy_token_type(&current, 0));
            return (0);
        }
        else if ((prev->type == TOKEN_REDIRECT_OUT) || (!is_redirection(&prev) && !current->next))
            return (prnt_sy_err(sy_token_type(&current, 1)), 0);
        else if (!current->next && is_redirection(&prev))
            return (prnt_sy_err(sy_token_type(&current, 0)), 0);
    }
    return (1);
}

int check_redirection_syntax(t_token *prev, t_token *current)
{
    if (is_redirection(&current))
    {
        if (!is_redirection(&prev) && !current->next)
            return (prnt_sy_err(sy_token_type(&current, 1)), 0);
        if (is_redirection(&prev) && !current->next)
        {
            if (prev->type == TOKEN_REDIRECT_IN && current->type == TOKEN_REDIRECT_OUT)
                return (prnt_sy_err(sy_token_type(&current, 1)), 0);
            else if (prev->type == TOKEN_HEREDOC && current->type == TOKEN_REDIRECT_IN)
                return (prnt_sy_err(sy_token_type(&current, 1)), 0);
            prnt_sy_err(sy_token_type(&current, 0));
            return (0);
        }
    }
    return (1);
}

int check_syntax_err(t_token *tokens)
{
    if (!tokens)
        return (1);
    t_token *prev;
    t_token *current;

    current = tokens;
    prev = NULL;
    if (is_pipe(&current))
        return (prnt_sy_err(sy_token_type(&current, 0)), 0);
    while (current)
    {
        if (!check_pipe_syntax(prev, current))
            return (0);
        if (!check_redirection_syntax(prev, current))
            return (0);
        prev = current;
        current = current->next;
    }
    return (1);
}
