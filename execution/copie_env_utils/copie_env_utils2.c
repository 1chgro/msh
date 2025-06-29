/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copie_env_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noel-baz <noel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:38:53 by noel-baz          #+#    #+#             */
/*   Updated: 2025/06/29 14:38:55 by noel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_key(char *str)
{
	char	*key;
	int		i;
	int		j;

	i = 0;
	while (str[i] && str[i] != '=' && str[i] != '+')
		i++;
	key = malloc(i + 1);
	if (!key)
		return (NULL);
	j = 0;
	while (j < i)
	{
		key[j] = str[j];
		j++;
	}
	key[j] = '\0';
	return (key);
}

static char	*extract_value_part(char *str, int start_pos)
{
	char	*value;
	int		len;
	int		j;

	len = 0;
	while (str[start_pos + len])
		len++;
	value = malloc(len + 1);
	if (!value)
		return (NULL);
	j = 0;
	while (str[start_pos])
	{
		value[j] = str[start_pos];
		start_pos++;
		j++;
	}
	value[j] = '\0';
	return (value);
}

char	*get_value(char *str)
{
	int	i;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (str[i] == '=')
		i++;
	else
		return (NULL);
	return (extract_value_part(str, i));
}

t_env	*create_node(char *key, char *value, int have_equal)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = key;
	if ((!value || value[0] == '\0') && have_equal)
	{
		if (value)
			free(value);
		node->value = ft_strdup("");
	}
	else
		node->value = value;
	if (have_equal)
		node->flag = 1;
	else
		node->flag = 0;
	node->index = -1;
	node->next = NULL;
	return (node);
}
