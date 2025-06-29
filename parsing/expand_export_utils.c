/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_export_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olachgue <olachgue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:47:49 by olachgue          #+#    #+#             */
/*   Updated: 2025/06/29 20:47:52 by olachgue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	if (!key || !*key)
		return (0);
	if (ft_isdigit(key[0]))
		return (1);
	while (key[i])
	{
		if (is_quote(key[i]))
			return (1);
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

char	**handle_equal_sign_case(char *str, char *equal_sign)
{
	char	**result;

	if (equal_sign == str)
	{
		result = malloc(sizeof(char *) * 2);
		if (!result)
			return (NULL);
		result[0] = ft_strdup(equal_sign);
		if (!result[0])
			return (free_arr(result), NULL);
		result[1] = NULL;
		return (result);
	}
	result = malloc(sizeof(char *) * 3);
	if (!result)
		return (NULL);
	result[0] = ft_strndup(str, equal_sign - str + 1);
	if (!result[0])
		return (free_arr(result), NULL);
	result[1] = ft_strdup(equal_sign + 1);
	if (!result[1])
		return (free(result[0]), free_arr(result), NULL);
	result[2] = NULL;
	return (result);
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
		result = handle_equal_sign_case(str, equal_sign);
		if (!result)
			return (NULL);
	}
	else
	{
		result = malloc(sizeof(char *) * 2);
		if (!result)
			return (NULL);
		result[0] = ft_strdup(str);
		if (!result[0])
			return (free_arr(result), NULL);
		result[1] = NULL;
	}
	return (result);
}
