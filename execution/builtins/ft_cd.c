/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noel-baz <noel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:48:52 by noel-baz          #+#    #+#             */
/*   Updated: 2025/06/30 19:26:36 by noel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	set_pwd_fallback(t_env **env, char *logical_pwd, char *path,
		int logical_dotdot)
{
	if (logical_dotdot)
	{
		set_env(env, "PWD", logical_pwd);
		take_store_pwd(logical_pwd);
	}
	else
	{
		set_env(env, "PWD", path);
		take_store_pwd(path);
	}
}

static int	update_pwd_env(t_env **env, char *logical_pwd, char *path,
		int logical_dotdot)
{
	char	*new_pwd;

	new_pwd = get_current_pwd();
	if (!new_pwd)
	{
		ft_putstr_fd("cd: error retrieving current directory: getcwd: "
			"cannot access parent directories: No such file or directory\n", 2);
		set_pwd_fallback(env, logical_pwd, path, logical_dotdot);
		return (1);
	}
	else
	{
		set_env(env, "PWD", new_pwd);
		take_store_pwd(new_pwd);
		free(new_pwd);
	}
	return (0);
}

static void	cleanup_and_exit(char *old_pwd, char *logical_pwd, char *path)
{
	if (old_pwd)
		free(old_pwd);
	if (logical_pwd)
		free(logical_pwd);
	if (path)
		free(path);
}

static int	process_cd_logic(char **s_cmd, t_env **env, char *old_pwd)
{
	char	*path;
	char	*logical_pwd;
	int		logical_dotdot;
	int		status;

	logical_pwd = NULL;
	path = NULL;
	path = determine_path(s_cmd, *env, old_pwd);
	if (!path)
		return (1);
	logical_dotdot = handle_dotdot_logic(path, old_pwd, &logical_pwd);
	if (logical_dotdot == 1)
		return (1);
	if (logical_dotdot == 2)
		logical_dotdot = 1;
	else
		logical_dotdot = 0;
	if (attempt_chdir(path, old_pwd) != 0)
		return (cleanup_and_exit(old_pwd, logical_pwd, path), 1);
	set_env(env, "OLDPWD", old_pwd);
	status = update_pwd_env(env, logical_pwd, path, logical_dotdot);
	cleanup_and_exit(old_pwd, logical_pwd, path);
	return (status);
}

int	ft_cd(char **s_cmd, t_env **env)
{
	char	*old_pwd;

	old_pwd = get_old_pwd_or_default();
	if (!old_pwd)
		return (1);
	return (process_cd_logic(s_cmd, env, old_pwd));
}
