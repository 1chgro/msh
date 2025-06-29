/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_expanding.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olachgue <olachgue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:48:26 by olachgue          #+#    #+#             */
/*   Updated: 2025/06/29 20:48:29 by olachgue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*build_key_value_result(char *key, char *value)
{
	char	*result;

	if (!key)
		return (NULL);
	result = NULL;
	result = ft_strjoin(result, key);
	if (value)
	{
		result = ft_strjoin_ws(result, value);
	}
	return (result);
}

char	*expand_key_value(char *str, t_glob_st *glob_strct, \
	int split_all_values)
{
	char	*result;
	char	*key;
	char	*value;
	char	**key_val;
	int		split_value;

	if (!str || !glob_strct)
		return (NULL);
	(1) && (result = NULL, key = NULL, value = NULL, split_value = 0);
	key_val = split_key_val(str);
	if (!key_val)
		return (NULL);
	if (key_val[0])
	{
		split_value = should_split_value(split_all_values, \
		key_val[0], key_val[1]);
		key = expand_key(key_val[0], glob_strct);
		if (!key)
			return (free_arr(key_val), NULL);
	}
	if (key_val[1])
		value = process_value_expansion(key_val[1], glob_strct, split_value);
	result = build_key_value_result(key, value);
	return (free_arr(key_val), free(value), free(key), result);
}

char	*get_word(char *line)
{
	char	*word;
	int		i;

	i = 0;
	while (line[i])
	{
		if (is_space(line[i]))
			break ;
		i++;
	}
	word = ft_strndup(line, i);
	return (word);
}

int	is_quoted_export(char *line)
{
	int		quote;
	char	*export_candidate;
	int		j;

	if (!line)
		return (0);
	quote = 0;
	j = 0;
	export_candidate = get_word(line);
	if (ft_strcmp(export_candidate, "export") == 0)
		return (free(export_candidate), 0);
	while (export_candidate[j])
	{
		if (export_candidate[j] == '\'' || export_candidate[j] == '"')
			quote++;
		j++;
	}
	return (free(export_candidate), quote);
}

char	*expand_export(char *line, t_glob_st *glob_strct)
{
	char	*result;
	char	**arr;
	int		i;
	int		split_all_values;
	char	*expanded;

	if (!line)
		return (NULL);
	(1) && (result = NULL, arr = NULL, split_all_values = 0, i = 0);
	arr = split_line_to_args(line);
	if (!arr)
		return (NULL);
	split_all_values = is_quoted_export(line);
	while (arr[i])
	{
		expanded = expand_key_value(arr[i], glob_strct, split_all_values);
		if (!expanded)
			return (free_arr(arr), NULL);
		result = ft_strjoin(result, expanded);
		free(expanded);
		if (!result)
			return (free_arr(arr), NULL);
		i++;
	}
	return (free_arr(arr), result);
}
