
#include "../minishell.h"

int	handle_operator(char *line, int *i, t_token **tokens)
{
	char	*op;
	int		len;
	t_token	*token;

	if (!line || !tokens)
		return (0);
	op = NULL;
	len = 1;
	if ((line[*i] == '<' && line[*i + 1] == '<') || \
		(line[*i] == '>' && line[*i + 1] == '>'))
		len = 2;
	op = malloc(len + 1);
	if (!op)
		return (0);
	ft_stringcpy(op, &line[*i], len);
	token = new_token(op, get_token_type(op));
	if (!add_token(tokens, token))
		return (free(token->value), free(token), 0);
	*i += len;
	return (1);
}

int	find_word_end(char *line, int start)
{
	int		i;
	char	q;

	i = start;
	while (line[i])
	{
		if (is_quote(line[i]))
		{
			q = line[i];
			skip_quotes(line, &i, q);
		}
		else if (!is_space(line[i]) && !is_operator(line[i]))
			i++;
		else
			break ;
	}
	return (i);
}

int	handle_word(char *line, int *i, t_token **tokens)
{
	int		start;
	int		end;
	char	*word;
	t_token	*token;

	if (!line || !tokens)
		return (0);
	word = NULL;
	start = *i;
	end = find_word_end(line, start);
	*i = end;
	(1) && (word = malloc(end - start + 1));
	if (!word)
		return (0);
	ft_stringcpy(word, &line[start], end - start);
	token = new_token(word, TOKEN_WORD);
	if (!token)
		return (0);
	if (!add_token(tokens, token))
		return (free(token->value), free(token), 0);
	return (1);
}

t_token	*tokenize(char *line)
{
	t_token	*tokens;
	int		i;

	if (!line)
		return (NULL);
	(1) && (tokens = NULL, i = 0);
	while (line[i])
	{
		if (is_space(line[i]))
		{
			i++;
			continue ;
		}
		if (is_operator(line[i]))
		{
			if (!handle_operator(line, &i, &tokens))
				return (free_tokens(tokens), NULL);
		}
		else
		{
			if (!handle_word(line, &i, &tokens))
				return (free_tokens(tokens), NULL);
		}
	}
	return (tokens);
}
