/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noel-baz <noel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:36:38 by noel-baz          #+#    #+#             */
/*   Updated: 2025/06/29 15:31:43 by noel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

char	*handle_tilde_path(t_env *env, char *arg, char *old_pwd)
{
	char	*home;
	char	*path;

	home = get_home(env);
	if (!home)
	{
		ft_putstr_fd("msh: cd: HOME not set\n", 2);
		free(old_pwd);
		return (NULL);
	}
	path = ft_strjoin_(home, arg + 1);
	if (!path)
	{
		ft_putstr_fd("msh: cd: cannot allocate memory\n", 2);
		free(old_pwd);
		return (NULL);
	}
	return (path);
}

char	*determine_path(char **s_cmd, t_env *env, char *old_pwd)
{
	if (!s_cmd[1])
		return (handle_no_args(env, old_pwd));
	else if (s_cmd[1][0] == '~' && ft_strlen(s_cmd[1]) == 1)
		return (handle_tilde_only(env, old_pwd));
	else if (s_cmd[1][0] == '-' && ft_strlen(s_cmd[1]) == 1)
		return (handle_dash_only(env, old_pwd));
	else if (s_cmd[1][0] == '~' && ft_strlen(s_cmd[1]) > 1)
		return (handle_tilde_path(env, s_cmd[1], old_pwd));
	else
		return (s_cmd[1]);
}

char	*create_logical_dotdot_path(char *old_pwd)
{
	char	*slashdotdot;

	if (old_pwd[ft_strlen(old_pwd) - 1] == '/')
		slashdotdot = ft_strjoin_(old_pwd, "..");
	else
		slashdotdot = ft_strjoin_(old_pwd, "/..");
	if (!slashdotdot)
		ft_putstr_fd("msh: cd: cannot allocate memory\n", 2);
	return (slashdotdot);
}

int	handle_dotdot_logic(char *path, char *old_pwd, char **logical_pwd)
{
	if (ft_strcmp(path, "..") == 0 && access(path, X_OK) == 0)
	{
		*logical_pwd = create_logical_dotdot_path(old_pwd);
		if (!*logical_pwd)
			return (1);
		return (2);
	}
	return (0);
}

char	*process_up_navigation(char *temp_path, char *to_rm)
{
	char	*new_path;

	if (to_rm == temp_path)
	{
		free(temp_path);
		return (ft_strdup("/"));
	}
	new_path = ft_strndup(temp_path, to_rm - temp_path);
	free(temp_path);
	return (new_path);
}
