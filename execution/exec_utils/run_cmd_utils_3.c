/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd_utils_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noel-baz <noel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:50:24 by noel-baz          #+#    #+#             */
/*   Updated: 2025/06/29 20:50:25 by noel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*my_getenv(char *name, t_env *env)
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

static void	handle_missing_shlvl(t_env *env)
{
	t_env	*shlvl_node;

	shlvl_node = create_node("SHLVL", "1", 1);
	append_node(&env, shlvl_node);
}

static int	calculate_new_shlvl(int shell_level)
{
	if (shell_level >= 999)
		return (0);
	else
		return (shell_level + 1);
}

void	handle_shell_level(t_env *env)
{
	int		shell_level;
	int		new_shlvl;
	int		is_valid;
	t_env	*shlvl_node;

	is_valid = 0;
	shlvl_node = get_shlvl(env);
	if (!shlvl_node || !shlvl_node->value)
	{
		handle_missing_shlvl(env);
		return ;
	}
	shell_level = ft_atoi_(shlvl_node->value, &is_valid);
	if (!is_valid)
		shell_level = 0;
	new_shlvl = calculate_new_shlvl(shell_level);
	update_node_value(shlvl_node, ft_itoa(new_shlvl), 0, 1);
}

int	is_directory(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
		return (0);
	return (S_ISDIR(statbuf.st_mode));
}
