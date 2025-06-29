/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olachgue <olachgue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:48:11 by olachgue          #+#    #+#             */
/*   Updated: 2025/06/29 20:48:14 by olachgue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*digit_expansion_hrdoc(char *line, int *i, char *result)
{
	char	tmp[2];

	(*i)++;
	tmp[0] = line[*i];
	tmp[1] = '\0';
	result = ft_strjoin_ws(result, tmp);
	(*i)++;
	return (result);
}

char	*var_expansion_hrdoc(char *line, int *i, \
				t_glob_st *glob_strct, char *result)
{
	char	*var_value;
	char	*var;
	int		j;

	(1) && (var_value = NULL, var = NULL, j = 0);
	(*i)++;
	if (ft_isdigit(line[*i]) && is_valid_char(line[*i + 1]))
		return (digit_expansion_hrdoc(line, i, result));
	if (line[*i] == '?')
	{
		var_value = ft_itoa(glob_strct->ext_stat);
		result = ft_strjoin_ws(result, var_value);
		return ((*i)++, free(var_value), result);
	}
	j = *i;
	while (line[j] && is_valid_char(line[j]))
		j++;
	var = ft_strndup(&line[*i], j - *i);
	var_value = my_getenv2(var, glob_strct->env);
	if (var_value)
		result = ft_strjoin_ws(result, var_value);
	else
		result = ft_strjoin_ws(result, "");
	*i = j;
	return (free(var), result);
}

char	*expand_heredoc_core(char *line, t_glob_st *glob_strct, char *result)
{
	int		i;
	int		pos;
	char	quote;
	char	tmp[2];

	(1) && (i = 0, quote = 0, pos = 0, tmp[0] = '\0', tmp[1] = '\0');
	while (line[i])
	{
		if (line[i] == '$' && (is_valid_char(line[i + 1]) || \
		line[i + 1] == '?') && quote != '\'')
			result = var_expansion_hrdoc(line, &i, glob_strct, result);
		else
		{
			tmp[0] = line[i++];
			tmp[1] = '\0';
			result = ft_strjoin_ws(result, tmp);
		}
	}
	return (result);
}

char	*expand_heredoc(char *line, t_glob_st *glob_strct)
{
	char	*result;

	if (!line)
		return (NULL);
	result = malloc(1);
	result[0] = '\0';
	return (expand_heredoc_core(line, glob_strct, result));
}
