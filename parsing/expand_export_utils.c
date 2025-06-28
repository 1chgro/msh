#include "../minishell.h"

int	check_if_export(char *line, t_glob_st *glob_strct)
{
	char	*tmp;
	char	*temp;

	if (!line)
		return (0);
	tmp = NULL;
	tmp = expand(line, glob_strct);
	if (!tmp)
		return (0);
	temp = tmp;
	tmp = remove_outer_quotes(tmp);
	free(temp);
	if (!tmp)
		return (0);
	tmp = ft_strtrim(tmp, " \t\n\r\v\f");
	if (!tmp)
		return (0);
	if (ft_strncmp("export", tmp, 6) == 0)
		return (free(tmp), 1);
	free(tmp);
	return (0);
}

int	check_key(char *key)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (key[i])
	{
		if (is_quote(key[i]))
		{
			if (quote == 0)
				quote = key[i];
			while (key[i] && key[i] != quote)
				i++;
			if (key[i] == '\0')
				return (0);
			if (key[i] == quote)
				return (1);
		}
		if (key[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

int	check_value(char *value)
{
	if (!value)
		return (0);
	if (ft_strchr(value, '$'))
		return (1);
	return (0);
}

char	**split_key_val(char *str)
{
	char	**result;
	char	*equal_sign;

	if (!str)
		return (NULL);
	equal_sign = ft_strchr(str, '=');
	if (equal_sign)
	{
		if (equal_sign == str)
		{
			result = malloc(sizeof(char *) * 2);
			if (!result)
				return (NULL);
			result[0] = ft_strdup(equal_sign);
			result[1] = NULL;
			free(str);
			return (result);
		}
		result = malloc(sizeof(char *) * 3);
		if (!result)
			return (NULL);
		result[0] = ft_strndup(str, equal_sign - str);
		result[1] = ft_strdup(equal_sign + 1);
		result[2] = NULL;
	}
	else
	{
		result = malloc(sizeof(char *) * 2);
		if (!result)
			return (NULL);
		result[0] = ft_strdup(str);
		result[1] = NULL;
	}
	free(str);
	return (result);
}

int	should_split_value(int split_all_values, char *key_str, char *value_str)
{
	if (split_all_values || (check_key(key_str) == 1 \
		&& check_value(value_str) == 1))
		return (1);
	return (0);
}
