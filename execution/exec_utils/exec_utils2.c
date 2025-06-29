#include "../../minishell.h"

int	open_heredoc(t_glob_st *glob_strct)
{
	t_cmd	*temp;
	int		status;

	temp = glob_strct->cmd;
	status = 0;
	while (temp)
	{
		status = process_cmd_heredocs(temp, glob_strct);
		if (status)
			return (status);
		temp = temp->next;
	}
	return (status);
}

void	close_cmd_heredocs(t_cmd *cmd)
{
	int	i;

	if (!cmd->files)
		return ;
	i = 0;
	while (cmd->files[i].filename)
	{
		if (cmd->files[i].type == HEREDOC && cmd->files[i].fd >= 0)
			close(cmd->files[i].fd);
		i++;
	}
}

void	close_heredoc(t_glob_st *glob_strct)
{
	t_cmd	*temp;

	temp = glob_strct->cmd;
	while (temp)
	{
		close_cmd_heredocs(temp);
		temp = temp->next;
	}
}

int	save_std_fds(int *saved_stdout, int *saved_stdin)
{
	*saved_stdout = dup(STDOUT_FILENO);
	*saved_stdin = dup(STDIN_FILENO);
	if (*saved_stdout < 0 || *saved_stdin < 0)
	{
		perror("dup error");
		return (1);
	}
	return (0);
}

int	handle_redirection_only(t_glob_st *glob_strct,
	int saved_stdout, int saved_stdin)
{
	int	status;

	status = redirection(glob_strct->cmd);
	if (status != 0)
	{
		glob_strct->ext_stat = status;
		restore_std_fds(saved_stdout, saved_stdin);
		return (status);
	}
	return (0);
}
