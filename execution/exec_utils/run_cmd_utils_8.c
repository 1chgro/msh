/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd_utils_8.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noel-baz <noel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:50:38 by noel-baz          #+#    #+#             */
/*   Updated: 2025/06/29 20:50:39 by noel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	setup_child_pipes(int prev_fd, t_cmd *cmd, int *p_fd)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (cmd->next)
	{
		dup2(p_fd[1], STDOUT_FILENO);
		close(p_fd[0]);
		close(p_fd[1]);
	}
}

void	execute_child_command(t_cmd *cmd, t_env *env, int last_ex)
{
	int	status;

	if (cmd->files)
	{
		status = redirection(cmd);
		if (status != 0)
			exit(status);
	}
	if (cmd->argv)
	{
		if (are_builtin(cmd->argv[0]))
		{
			status = run_builtin(cmd->argv, &env, last_ex);
			exit(status);
		}
		else
		{
			exec(cmd->argv, env);
			exit(1);
		}
	}
	exit(0);
}

int	get_exit_status(int status)
{
	int	sig;
	int	last_status;

	sig = 0;
	last_status = 0;
	if (WIFEXITED(status))
		last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			last_status = SIGINT + 128;
		else if (sig == SIGQUIT)
		{
			dup2(2, 1);
			printf("Quit: %d\n", SIGQUIT);
			dup2(1, 2);
			last_status = SIGQUIT + 128;
		}
	}
	return (last_status);
}

void	wait_for_remaining_processes(void)
{
	while (wait(NULL) != -1)
		;
}
