/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noel-baz <noel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:49:03 by noel-baz          #+#    #+#             */
/*   Updated: 2025/06/29 20:49:04 by noel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	process_export_arg(char *arg, t_env **env)
{
	char	*key;
	char	*value;
	int		append;
	int		have_equal;
	t_env	*node;

	have_equal = 0;
	append = check_append_op(arg);
	key = get_key(arg);
	if (!validate_export_arg(key, arg, append))
		return (0);
	if (ft_strchr(arg, '='))
		have_equal = 1;
	value = get_value(arg);
	node = find_existing_node(*env, key);
	if (node)
	{
		update_node_value(node, value, append, have_equal);
		free(key);
		return (1);
	}
	else
		return (handle_new_node(env, key, value, have_equal));
}

static int	count_list_size(t_env *env)
{
	int		lst_size;
	t_env	*temp;

	lst_size = 0;
	temp = env;
	while (temp)
	{
		lst_size++;
		temp = temp->next;
	}
	return (lst_size);
}

static int	handle_no_args_export(t_env *env)
{
	int	lst_size;

	lst_size = count_list_size(env);
	print_export(env, lst_size);
	return (0);
}

static int	process_all_args(char **s_cmd, t_env **env)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	while (s_cmd[i])
	{
		if (!process_export_arg(s_cmd[i], env))
			status = 1;
		i++;
	}
	return (status);
}

int	ft_export(char **s_cmd, t_env **env)
{
	make_index(env);
	if (!s_cmd[1])
		return (handle_no_args_export(*env));
	return (process_all_args(s_cmd, env));
}
