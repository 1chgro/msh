/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noel-baz <noel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:41:04 by noel-baz          #+#    #+#             */
/*   Updated: 2025/06/29 14:41:06 by noel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	process_single_command(t_cmd *cmd, t_env *env,
	int last_ex, int *prev_fd)
{
	pid_t	pid;
	int		p_fd[2];

	if (setup_pipes(cmd, p_fd, *prev_fd) == 1)
		return (-2);
	pid = fork();
	if (pid == -1)
	{
		(cmd->next) && (close(p_fd[0]), close(p_fd[1]));
		if (*prev_fd != -1)
			close(*prev_fd);
		return (-1);
	}
	if (pid == 0)
		(setup_child_signals(), setup_child_pipes(*prev_fd, cmd, p_fd),
			execute_child_command(cmd, env, last_ex));
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->next)
	{
		close(p_fd[1]);
		*prev_fd = p_fd[0];
	}
	return (pid);
}

int	run_pipeline(t_cmd *cmd, t_env *env, int last_ex)
{
	pid_t	pid[1392];
	int		prev_fd;
	int		status;
	int		i;
	int		result;

	prev_fd = -1;
	i = 0;
	while (cmd)
	{
		result = process_single_command(cmd, env, last_ex, &prev_fd);
		if (result == -2)
			return (handle_pipe_error(pid, i));
		if (result == -1)
			return (handle_fork_error(pid, i));
		pid[i] = result;
		cmd = cmd->next;
		i++;
	}
	if (prev_fd != -1)
		close(prev_fd);
	waitpid(pid[i - 1], &status, 0);
	wait_for_remaining_processes();
	return (get_exit_status(status));
}

static void	single_child(t_cmd *cmd, t_env *env)
{
	int	status;

	status = 0;
	if (cmd->files)
	{
		status = redirection(cmd);
		if (status != 0)
			exit(status);
	}
	if (cmd->argv)
	{
		exec(cmd->argv, env);
	}
	exit(0);
}

int	run_single_cmd(t_cmd *cmd, t_env *env)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid == -1)
	{
		kill(pid, SIGKILL);
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		setup_child_signals();
		single_child(cmd, env);
	}
	waitpid(pid, &status, 0);
	return (get_exit_status(status));
}

int	run_cmd(t_cmd *cmd, t_env *env, int last_ex)
{
	if (!cmd)
	{
		perror("invalid command");
		return (1);
	}
	if (!cmd->next)
		return (run_single_cmd(cmd, env));
	return (run_pipeline(cmd, env, last_ex));
}
