/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olachgue <olachgue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:11:09 by olachgue          #+#    #+#             */
/*   Updated: 2025/06/29 14:11:10 by olachgue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_redirection_type	get_cmd_red_type(t_token_type type)
{
	if (type == TOKEN_REDIRECT_IN)
		return (REDIRECT_IN);
	else if (type == TOKEN_REDIRECT_OUT)
		return (REDIRECT_OUT);
	else if (type == TOKEN_APPEND)
		return (APPEND);
	else if (type == TOKEN_HEREDOC)
		return (HEREDOC);
	return (-1);
}

int	is_valid_char(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || \
		(c >= '0' && c <= '9') || (c == '_'));
}

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

char	*my_getenv2(char *name, t_env *env)
{
	t_env	*temp;

	temp = env;
	while (temp)
	{
		if (ft_strcmp(temp->key, name) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return (NULL);
}
