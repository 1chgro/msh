#include "../../minishell.h"

static int	handle_heredoc(t_red *file)
{
	if (file->fd < 0 || dup2(file->fd, STDIN_FILENO) < 0)
	{
		perror("dup2 or heredoc failed");
		if (file->fd >= 0)
			close(file->fd);
		return (1);
	}
	return (0);
}

static int	handle_redirect_in(t_red *file)
{
	file->fd = open(file->filename, O_RDONLY);
	if (file->fd < 0)
	{
		perror("msh: ");
		return (1);
	}
	dup2(file->fd, STDIN_FILENO);
	close(file->fd);
	return (0);
}

static int	handle_redirect_out(t_red *file)
{
	if (ft_strcmp(file->filename, "/dev/stdout") == 0)
		return (0);
	file->fd = open(file->filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (file->fd < 0)
	{
		perror("msh");
		return (1);
	}
	dup2(file->fd, STDOUT_FILENO);
	close(file->fd);
	return (0);
}

static int	handle_append(t_red *file)
{
	file->fd = open(file->filename, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (file->fd < 0)
	{
		perror("msh");
		return (1);
	}
	dup2(file->fd, STDOUT_FILENO);
	close(file->fd);
	return (0);
}

int	redirection(t_cmd *cmd)
{
	int	i;
	int	result;
	int	size;

	(1) && (i = -1, size = 0);
	while (cmd->files[++i].filename || cmd->files[i].ambiguous_flg)
		size++;
	i = 0;
	while (i < size)
	{
		if (cmd->files[i].ambiguous_flg)
			return (print_error(cmd->files[i].filename, AMBIG), 1);
		if (cmd->files[i].type == HEREDOC)
			result = handle_heredoc(&cmd->files[i]);
		else if (cmd->files[i].type == REDIRECT_IN)
			result = handle_redirect_in(&cmd->files[i]);
		else if (cmd->files[i].type == REDIRECT_OUT)
			result = handle_redirect_out(&cmd->files[i]);
		else if (cmd->files[i].type == APPEND)
			result = handle_append(&cmd->files[i]);
		if (result != 0)
			return (result);
		i++;
	}
	return (0);
}
