/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noel-baz <noel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:36:12 by noel-baz          #+#    #+#             */
/*   Updated: 2025/06/29 14:36:15 by noel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

char	*get_home(t_env *env)
{
	t_env	*temp;

	temp = env;
	while (temp)
	{
		if (ft_strcmp(temp->key, "HOME") == 0)
			return (temp->value);
		temp = temp->next;
	}
	return (NULL);
}

t_env	*find_env_key(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

t_env	*create_new_env_node(char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->flag = 1;
	new_node->index = 0;
	new_node->next = NULL;
	return (new_node);
}

void	update_existing_env(t_env *current, char *value)
{
	char	*new_value;

	new_value = ft_strdup(value);
	if (!new_value)
		return ;
	free(current->value);
	current->value = new_value;
	current->flag = 1;
}

void	set_env(t_env **env, char *key, char *value)
{
	t_env	*current;
	t_env	*new_node;

	current = find_env_key(*env, key);
	if (current)
	{
		update_existing_env(current, value);
		return ;
	}
	new_node = create_new_env_node(key, value);
	if (!new_node)
		return ;
	append_node(env, new_node);
}
