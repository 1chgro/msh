#include "../../minishell.h"

int	setup_pipes(t_cmd *cmd, int *p_fd, int prev_fd)
{
	if (cmd->next && pipe(p_fd) == -1)
	{
		if (cmd->next)
		{
			close(p_fd[0]);
			close(p_fd[1]);
		}
		if (prev_fd != -1)
			close(prev_fd);
		return (1);
	}
	return (0);
}
