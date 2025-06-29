/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noel-baz <noel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:38:40 by noel-baz          #+#    #+#             */
/*   Updated: 2025/06/29 14:38:41 by noel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	free_current(t_env *current)
{
	free(current->key);
	free(current->value);
	free(current);
}

static void	remove_variable(char *key, t_env **env)
{
	t_env	*current;
	t_env	*prev;

	if (!env || !*env)
		return ;
	current = *env;
	prev = NULL;
	if (!current)
		return ;
	if (ft_strcmp(current->key, key) == 0)
	{
		*env = current->next;
		free_current(current);
		return ;
	}
	while (current && ft_strcmp(current->key, key) != 0)
	{
		prev = current;
		current = current->next;
	}
	if (!current)
		return ;
	prev->next = current->next;
	free_current(current);
}

int	check_and_remove_varible(char *arg, t_env **env)
{
	t_env	*temp;
	int		flag;

	(1) && (flag = 0, temp = NULL);
	if (!is_valid_identifier(arg))
	{
		dup2(2, 1);
		printf("unset: `%s':not a valid identifier\n", arg);
		dup2(1, 2);
		return (1);
	}
	temp = *env;
	while (temp)
	{
		if (ft_strcmp(arg, temp->key) == 0)
		{
			flag = 1;
			break ;
		}
		temp = temp->next;
	}
	if (flag == 1)
		remove_variable(arg, env);
	return (0);
}

int	ft_unset(char **s_cmd, t_env **env)
{
	t_env	*temp;
	int		flag;
	int		i;
	int		status;

	if (!env || !*env)
		return (1);
	(1) && (i = -1, flag = 0, status = 0, temp = NULL);
	if (!s_cmd[1])
		return (0);
	while (s_cmd[++i])
	{
		if (check_and_remove_varible(s_cmd[i], env))
			status = 1;
	}
	return (status);
}
