/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noel-baz <noel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:48:57 by noel-baz          #+#    #+#             */
/*   Updated: 2025/06/29 20:48:58 by noel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_env(char **s_cmd, t_env *env)
{
	t_env	*temp;

	temp = NULL;
	if (!env)
		return (0);
	temp = env;
	(void)s_cmd;
	while (temp)
	{
		if (temp->flag == 1)
			printf("%s=%s\n", temp->key, temp->value);
		temp = temp->next;
	}
	return (0);
}
