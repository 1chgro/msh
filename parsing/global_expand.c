
#include "../minishell.h"

static char	*handle_regular_variable_expansion(char *line, \
		int *i, t_glob_st *glob_strct, char *result)
{
	char	*var_value;
	char	*var;
	int		j;

	j = *i;
	while (line[j] && is_valid_char(line[j]))
		j++;
	var = ft_strndup(&line[*i], j - *i);
	var_value = my_getenv2(var, glob_strct->env);
	var_value = ft_strdup(var_value);
	var_value = replace_quotes(var_value);
	if (var_value)
		result = ft_strjoin_ws(result, var_value);
	else
		result = ft_strjoin_ws(result, "");
	*i = j;
	free(var);
	free(var_value);
	return (result);
}

char	*handle_variable_expansion(char *line, int *i, \
		t_glob_st *glob_strct, char *result)
{
	(*i)++;
	if (ft_isdigit(line[*i]) && is_valid_char(line[*i + 1]))
	{
		return (handle_digit_expansion(line, i, result));
	}
	if (line[*i] == '?')
	{
		return (handle_exit_status_expansion(i, glob_strct, result));
	}
	return (handle_regular_variable_expansion(line, i, glob_strct, result));
}

static int	handle_dollar_case(char *line, int *i, char quote)
{
	if (line[*i] == '$' && line[*i + 1] == '\0')
		return (0);
	if (line[*i] == '$' && line[*i + 1] == '$')
	{
		*i += 2;
		return (1);
	}
	else if (line[*i] == '$' && !is_valid_char(line[*i + 1]) && \
			!is_space(line[*i + 1]) && quote == 0)
	{
		(*i)++;
		return (1);
	}
	return (0);
}

char	*expand_core(char *line, t_glob_st *glob_strct)
{
	int		i;
	int		pos;
	char	quote;
	char	tmp[2];
	char	*result;

	(1) && (i = 0, quote = 0, pos = 0, tmp[1] = '\0', result = NULL);
	while (line[i])
	{
		handle_quotes(line, i, &quote, &pos);
		if (line[i] == '$' && (is_valid_char(line[i + 1]) || \
				line[i + 1] == '?') && quote != '\'')
			result = handle_variable_expansion(line, &i, glob_strct, result);
		else
		{
			if (handle_dollar_case(line, &i, quote))
				continue ;
			if (line[i])
				tmp[0] = line[i++];
			result = ft_strjoin_ws(result, tmp);
			if (!result)
				return (NULL);
		}
	}
	return (result);
}

char	*expand(char *line, t_glob_st *glob_strct)
{
	char	*result;

	if (!line)
		return (NULL);
	result = expand_core(line, glob_strct);
	return (result);
}
