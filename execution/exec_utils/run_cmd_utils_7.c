#include "../../minishell.h"

static void	handle_execve_failure(char **cmd, char *path, t_env *env)
{
	try_bash_execution(cmd, path, env);
	print_error(cmd[0], ": execve failed : ");
	perror("");
	free(path);
	exit(1);
}

void	exec(char **cmd, t_env *env)
{
	char	*path;

	if (!cmd || !cmd[0])
		exit(1);
	path = get_path(cmd[0], env);
	if (!path)
		exit(127);
	handle_minishell_exec(path, env);
	if (execve(path, cmd, struct_to_array(env)) == -1)
		handle_execve_failure(cmd, path, env);
}

static void	cleanup_processes(pid_t *pid, int count)
{
	int	j;

	j = 0;
	while (j < count)
	{
		kill(pid[j], SIGKILL);
		waitpid(pid[j], NULL, 0);
		j++;
	}
}

int	handle_pipe_error(pid_t *pid, int i)
{
	cleanup_processes(pid, i);
	perror("msh: ");
	return (1);
}

int	handle_fork_error(pid_t *pid, int i)
{
	cleanup_processes(pid, i);
	perror("msh: ");
	return (1);
}
