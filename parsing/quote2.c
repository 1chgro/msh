#include "../minishell.h"

char	get_replacement_char(char c, char quote)
{
	if (!is_quote(c))
		return (c);
	if (!quote || c == quote)
		return (c);
	if (c == '\'')
		return ('\x01');
	else if (c == '\"')
		return ('\x02');
	return (c);
}

char	process_character(char c, char *quote)
{
	char	replacement;

	if (is_quote(c))
	{
		if (!*quote)
			*quote = c;
		else if (c == *quote)
			*quote = 0;
	}
	replacement = get_replacement_char(c, *quote);
	return (replacement);
}

char	*replace_value_quotes(char *value)
{
	char	*new_value;
	char	tmp[2];
	int		i;
	char	quote;

	if (!value)
		return (NULL);
	(1) && (i = 0, quote = 0, new_value = NULL);
	tmp[1] = '\0';
	while (value[i])
	{
		tmp[0] = process_character(value[i], &quote);
		new_value = ft_strjoin_ws(new_value, tmp);
		i++;
	}
	free(value);
	return (new_value);
}

char	*replace_quotes(char *value)
{
	char	*new_value;
	char	tmp[2];
	int		i;

	if (!value)
		return (NULL);
	tmp[1] = '\0';
	i = 0;
	new_value = NULL;
	while (value[i])
	{
		if (value[i] == '\'')
			tmp[0] = '\x01';
		else if (value[i] == '\"')
			tmp[0] = '\x02';
		else
			tmp[0] = value[i];
		tmp[1] = '\0';
		new_value = ft_strjoin_ws(new_value, tmp);
		i++;
	}
	return (free(value), new_value);
}

char	*restore_quotes(char *value)
{
	char	*new_value;
	char	tmp[2];
	int		i;

	if (!value)
		return (NULL);
	if (!*value)
		return (free(value), ft_strdup(""));
	new_value = NULL;
	tmp[1] = '\0';
	i = 0;
	while (value[i])
	{
		if (value[i] == '\x01')
			tmp[0] = '\'';
		else if (value[i] == '\x02')
			tmp[0] = '\"';
		else
			tmp[0] = value[i];
		tmp[1] = '\0';
		new_value = ft_strjoin_ws(new_value, tmp);
		i++;
	}
	return (free(value), new_value);
}
