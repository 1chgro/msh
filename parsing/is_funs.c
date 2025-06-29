
#include "../minishell.h"

int	is_space(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	is_operator(char c)
{
	return (c == '|' || c == '>' || c == '<');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

int	is_redirection(t_token **token)
{
	if (!*token)
		return (0);
	if ((*token)->type == TOKEN_APPEND
		|| (*token)->type == TOKEN_HEREDOC || (*token)->type == \
			TOKEN_REDIRECT_IN || (*token)->type == TOKEN_REDIRECT_OUT)
		return (1);
	return (0);
}

int	is_pipe(t_token **token)
{
	if (!*token)
		return (0);
	if ((*token)->type == TOKEN_PIPE)
		return (1);
	return (0);
}
