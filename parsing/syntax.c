#include "../minishell.h"

char	*sy_token_type(t_token **token, int n_Line_flg)
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

void	prnt_sy_err(char *value)
{
	ft_putstr_fd("msh: syntax error near unexpected token `", 2);
	ft_putstr_fd(value, 2);
	ft_putstr_fd("'\n", 2);
}

int	check_pipe_syntax(t_token *prev, t_token *current)
{
	if (is_pipe(&current))
	{
		if (is_redirection(&prev))
		{
			prnt_sy_err(sy_token_type(&current, 0));
			return (0);
		}
		else if (is_pipe(&prev))
		{
			prnt_sy_err(sy_token_type(&current, 0));
			return (0);
		}
		else if ((prev->type == TOKEN_REDIRECT_OUT) || \
			(!is_redirection(&prev) && !current->next))
			return (prnt_sy_err(sy_token_type(&current, 1)), 0);
		else if (!current->next && is_redirection(&prev))
			return (prnt_sy_err(sy_token_type(&current, 0)), 0);
	}
	return (1);
}

int	check_redirection_syntax(t_token *prev, t_token *current)
{
	if (is_redirection(&current))
	{
		if (!is_redirection(&prev) && !current->next)
			return (prnt_sy_err(sy_token_type(&current, 1)), 0);
		if (is_redirection(&prev) && !current->next)
		{
			if (prev->type == TOKEN_REDIRECT_IN && \
				current->type == TOKEN_REDIRECT_OUT)
				return (prnt_sy_err(sy_token_type(&current, 1)), 0);
			else if (prev->type == TOKEN_HEREDOC && \
				current->type == TOKEN_REDIRECT_IN)
				return (prnt_sy_err(sy_token_type(&current, 1)), 0);
			prnt_sy_err(sy_token_type(&current, 0));
			return (0);
		}
		if (is_redirection(&prev) && \
			is_redirection(&current))
		{
			prnt_sy_err(sy_token_type(&current, 0));
			return (0);
		}
	}
	return (1);
}

int	check_syntax_err(t_glob_st *glob_strct)
{
	t_token	*prev;
	t_token	*current;

	if (!glob_strct->tokens)
		return (0);
	current = glob_strct->tokens;
	prev = NULL;
	if (is_pipe(&current))
	{
		prnt_sy_err(sy_token_type(&current, 0));
		return (1);
	}
	while (current)
	{
		if (!check_pipe_syntax(prev, current))
			return (1);
		if (!check_redirection_syntax(prev, current))
			return (1);
		prev = current;
		current = current->next;
	}
	// print_tokens(glob_strct->tokens);
	return (0);
}
