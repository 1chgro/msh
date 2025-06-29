/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd_utils_5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noel-baz <noel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:39:35 by noel-baz          #+#    #+#             */
/*   Updated: 2025/06/29 14:39:36 by noel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*handle_absolute_path(char *cmd)
{
	if (access(cmd, F_OK) != 0)
	{
		ft_putstr_fd("msh: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
		if (errno == ENOTDIR)
		{
			ft_putstr_fd("Not a directory\n", 2);
			exit(126);
		}
		else if (errno == ENOENT)
			ft_putstr_fd("No such file or directory\n", 2);
		else
			perror("");
		exit(127);
	}
	if (is_directory(cmd))
		print_error_exit(cmd, ": is a directory\n", 126);
	if (access(cmd, X_OK) != 0)
		print_error_exit(cmd, ": Permission denied\n", 126);
	return (ft_strdup(cmd));
}

static char	*search_in_path(char *cmd, char *path, int null_path)
{
	char	**allpath;
	char	*result;

	allpath = ft_split(path, ':');
	if (!allpath)
	{
		perror("msh: ");
		return (NULL);
	}
	result = check_command_path(allpath, cmd);
	if (!result && null_path)
	{
		if (allpath)
			free_split(allpath);
		print_error(cmd, ": No such file or directory\n");
	}
	else if (!result)
	{
		free_split(allpath);
		print_error(cmd, ": command not found\n");
	}
	return (result);
}

char	*get_path(char *cmd, t_env *env)
{
	char	*path;
	int		null_path;

	null_path = 0;
	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (handle_absolute_path(cmd));
	path = my_getenv("PATH", env);
	if (!path)
	{
		null_path = 1;
		path = ft_strdup(".");
	}
	return (search_in_path(cmd, path, null_path));
}

void	handle_minishell_exec(char *path, t_env *env)
{
	if (ft_strcmp(path, "./minishell") == 0)
		handle_shell_level(env);
}

void	try_bash_execution(char **cmd, char *path, t_env *env)
{
	char	**new_arg;

	new_arg = malloc((sizeof(char *) * 3));
	if (!new_arg)
		perror("malloc");
	new_arg[0] = ft_strdup("bash");
	new_arg[1] = path;
	new_arg[2] = NULL;
	if (execve("/bin/bash", new_arg, struct_to_array(env)) == -1)
	{
		print_error(cmd[0], ": execve failed : ");
		perror("");
		free_split(new_arg);
		if (path)
			free(path);
		exit(1);
	}
}
