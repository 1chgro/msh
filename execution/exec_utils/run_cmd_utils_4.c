#include "../../minishell.h"

int	is_file(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
		return (0);
	return (S_ISREG(statbuf.st_mode));
}

void	print_error_exit(char *cmd, char *msg, int exit_num)
{
	ft_putstr_fd("msh: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(msg, 2);
	exit(exit_num);
}

static char	*build_exec_path(char *path_part, char *cmd)
{
	char	*exec;
	char	*temp;

	temp = ft_strjoin_(path_part, "/");
	if (!temp)
		return (NULL);
	exec = ft_strjoin_(temp, cmd);
	free(temp);
	return (exec);
}

char	*check_command_path(char **allpath, char *cmd)
{
	int		i;
	char	*exec;

	i = -1;
	while (allpath && allpath[++i])
	{
		exec = build_exec_path(allpath[i], cmd);
		if (!exec)
			continue ;
		if (access(exec, F_OK) == 0)
		{
			if (is_directory(exec))
			{
				free(exec);
				continue ;
			}
			if (access(exec, X_OK) == 0)
				return (free_split(allpath), exec);
			else if (is_file(cmd) && access(exec, X_OK) != 0)
				print_error_exit(exec, ": Permission denied\n", 126);
		}
		free(exec);
	}
	return (NULL);
}

void	print_error(char *cmd, char *msg)
{
	ft_putstr_fd("msh: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(msg, 2);
}
