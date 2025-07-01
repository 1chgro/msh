/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copie_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noel-baz <noel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:50:44 by noel-baz          #+#    #+#             */
/*   Updated: 2025/07/01 08:41:53 by noel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	append_node(t_env **head, t_env *node)
{
	t_env	*tail;

	if (!node)
		return ;
	if (!*head)
	{
		*head = node;
		return ;
	}
	tail = *head;
	while (tail->next)
		tail = tail->next;
	tail->next = node;
}

char	**fill_env(void)
{
	char	**env;
	char	*pwd;

	env = malloc(sizeof(char *) * 6);
	if (!env)
		return (NULL);
	pwd = get_current_pwd();
	if (!pwd)
		return (NULL);
	env[0] = ft_strdup("OLDPWD=");
	if (!env[0])
		return (free(pwd), NULL);
	env[1] = ft_strjoin_("PWD=", pwd);
	if (!env[1])
		return (free(pwd), free_arr(env), NULL);
	env[2] = ft_strdup("SHLVL=1");
	if (!env[2])
		return (free(pwd), free_arr(env), NULL);
	env[3] = ft_strdup("_=/usr/bin/env");
	if (!env[3])
		return (free(pwd), free_arr(env), NULL);
	env[4] = ft_strdup("PATH=/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.");
	if (!env[4])
		return (free(pwd), free_arr(env), NULL);
	return (env[5] = NULL, free(pwd), env);
}

static void	creat_apend_node(int empty, t_env *node, t_env **c_env)
{
	if (empty && ft_strcmp(node->key, "PATH") == 0)
		node->flag = 2;
	append_node(c_env, node);
}

static int	process_env_entry(t_env **c_env, char *env_str, int empty)
{
	t_env	*node;
	char	*key;
	char	*value;
	int		have_equal;

	have_equal = 0;
	if (ft_strchr(env_str, '='))
		have_equal = 1;
	key = get_key(env_str);
	if (!key)
		return (0);
	if (ft_strcmp(key, "OLDPWD") == 0)
		(1) && (have_equal = 0, value = NULL);
	else
	{
		value = get_value(env_str);
		if (!value)
			return (free(key), 0);
	}
	node = create_node(key, value, have_equal);
	if (!node)
		return (free(key), free(value), 0);
	creat_apend_node(empty, node, c_env);
	return (1);
}

int	copie_env(t_env **c_env, char **env)
{
	int	empty;
	int	i;

	*c_env = NULL;
	empty = 0;
	if (!*env)
	{
		empty = 1;
		env = fill_env();
		if (!env)
			return (0);
	}
	i = 0;
	while (env[i])
	{
		if (!process_env_entry(c_env, env[i], empty))
			return (0);
		i++;
	}
	if (empty)
		free_split(env);
	return (1);
}
