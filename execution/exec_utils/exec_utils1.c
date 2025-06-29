/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noel-baz <noel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:38:59 by noel-baz          #+#    #+#             */
/*   Updated: 2025/06/29 14:39:01 by noel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	are_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

static int	execute_builtin(char *cmd, char **s_cmd, t_env **env, int last_ex)
{
	if (ft_strcmp(cmd, "cd") == 0)
		return (ft_cd(s_cmd, env));
	if (ft_strcmp(cmd, "echo") == 0)
		return (ft_echo(s_cmd));
	if (ft_strcmp(cmd, "pwd") == 0)
		return (ft_pwd(*env));
	if (ft_strcmp(cmd, "export") == 0)
		return (ft_export(s_cmd, env));
	if (ft_strcmp(cmd, "unset") == 0)
		return (ft_unset(s_cmd, env));
	if (ft_strcmp(cmd, "env") == 0)
		return (ft_env(s_cmd, *env));
	if (ft_strcmp(cmd, "exit") == 0)
		return (ft_exit(s_cmd, last_ex));
	return (127);
}

int	run_builtin(char **s_cmd, t_env **env, int last_ex)
{
	if (!s_cmd || !s_cmd[0])
	{
		perror("invalid built-in command");
		return (1);
	}
	return (execute_builtin(s_cmd[0], s_cmd, env, last_ex));
}

static int	process_heredoc_file(t_red *file, t_glob_st *glob_strct)
{
	int	status;

	status = here_doc(file->filename, &file->fd, &file->expand_flg, glob_strct);
	if (status)
		return (status);
	if (file->fd < 0)
	{
		perror("msh : ");
		return (status);
	}
	return (0);
}

int	process_cmd_heredocs(t_cmd *cmd, t_glob_st *glob_strct)
{
	int	i;
	int	status;

	if (!cmd->files)
		return (0);
	i = 0;
	while (cmd->files[i].filename)
	{
		if (cmd->files[i].type == HEREDOC)
		{
			status = process_heredoc_file(&cmd->files[i], glob_strct);
			if (status)
				return (status);
		}
		i++;
	}
	return (0);
}
