#include "../minishell.h"

static int	count_commands(t_cmd *cmd)
{
	int		size;
	t_cmd	*temp;

	size = 0;
	temp = cmd;
	while (temp)
	{
		size++;
		temp = temp->next;
	}
	return (size);
}

void	restore_std_fds(int saved_stdout, int saved_stdin)
{
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdout);
	close(saved_stdin);
}

static int	handle_builtin_command(t_glob_st *glob_strct,
	int saved_stdout, int saved_stdin)
{
	int	status;

	if (glob_strct->cmd->files)
	{
		status = redirection(glob_strct->cmd);
		if (status != 0)
		{
			glob_strct->ext_stat = status;
			restore_std_fds(saved_stdout, saved_stdin);
			return (status);
		}
	}
	glob_strct->ext_stat = run_builtin(glob_strct->cmd->argv,
			&glob_strct->env, glob_strct->ext_stat);
	return (0);
}

static int	handle_single_command(t_glob_st *glob_strct,
	int saved_stdout, int saved_stdin)
{
	if (!glob_strct->cmd->argv)
	{
		if (glob_strct->cmd->files)
		{
			if (handle_redirection_only(glob_strct, saved_stdout, saved_stdin))
				return (close_heredoc(glob_strct), glob_strct->ext_stat);
		}
		glob_strct->ext_stat = 0;
	}
	else if (are_builtin(glob_strct->cmd->argv[0]))
	{
		if (handle_builtin_command(glob_strct, saved_stdout, saved_stdin))
			return (glob_strct->ext_stat);
	}
	else
		glob_strct->ext_stat = run_cmd(glob_strct->cmd, glob_strct->env,
				glob_strct->ext_stat);
	return (0);
}

int	msh_execute(t_glob_st *glob_strct)
{
	int	size;
	int	saved_stdout;
	int	saved_stdin;
	int	result;

	if (!glob_strct->cmd)
		return (1);
	size = count_commands(glob_strct->cmd);
	if (save_std_fds(&saved_stdout, &saved_stdin))
		return (1);
	if (size == 1)
	{
		result = handle_single_command(glob_strct, saved_stdout, saved_stdin);
		if (result != 0)
			return (result);
	}
	else
		glob_strct->ext_stat = run_cmd(glob_strct->cmd, glob_strct->env,
				glob_strct->ext_stat);
	return (restore_std_fds(saved_stdout, saved_stdin),
		close_heredoc(glob_strct), glob_strct->ext_stat);
}
