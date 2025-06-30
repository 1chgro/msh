/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olachgue <olachgue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:48:17 by olachgue          #+#    #+#             */
/*   Updated: 2025/06/30 20:09:01 by olachgue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_for_ambgu(char *name)
{
	int	i;

	if (!name || !*name)
		return (1);
	i = 0;
	while (name[i])
	{
		if (is_space(name[i]))
			return (1);
		i++;
	}
	return (0);
}

char	*ext_stat_expanding(int *i, \
		t_glob_st *glob_strct, char *result)
{
	char	*var_value;

	var_value = ft_itoa(glob_strct->ext_stat);
	result = ft_strjoin_ws(result, var_value);
	(*i)++;
	free(var_value);
	return (result);
}

char	*add_quotes(char *value)
{
	char	*result;

	if (!value)
		return (NULL);
	result = NULL;
	if (value[0] == '"' || value[0] == '\'')
		result = ft_strdup(value);
	else
	{
		result = malloc(sizeof(char) * (ft_strlen(value) + 3));
		if (!result)
			return (NULL);
		result[0] = '"';
		ft_stringcpy(&result[1], value, ft_strlen(value));
		result[ft_strlen(value) + 1] = '"';
		result[ft_strlen(value) + 2] = '\0';
	}
	free(value);
	return (result);
}
