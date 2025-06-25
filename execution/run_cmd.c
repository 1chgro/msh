#include "../minishell.h"

char	*ft_strjoin_(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	j = 0;
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	str = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!str)
		return (free(s1), NULL);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[j])
		str[i++] = s2[j++];
	str[i] = '\0';
	// free(s1);
	return (str);
}
t_env	*get_shlvl(t_env *env)
{
	t_env	*temp;

	temp = env;
	while (temp)
	{
		if (ft_strcmp(temp->key, "SHLVL") == 0)
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}

static int	count_env_size(t_env *env)
{
	int		size_lst;
	t_env	*temp;

	size_lst = 0;
	temp = env;
	while (temp)
	{
		size_lst++;
		temp = temp->next;
	}
	return (size_lst);
}

static void	fill_env_array(char **arr, t_env *env, int *i)
{
	char	*str;
	t_env	*temp;

	temp = env;
	while (temp)
	{
		str = ft_strjoin_(temp->key, "=");
		arr[(*i)++] = ft_strjoin_(str, temp->value);
		temp = temp->next;
	}
}

char	**struct_to_array(t_env *env)
{
	int		i;
	int		size_lst;
	char	**arr;

	i = 0;
	size_lst = count_env_size(env);
	arr = malloc(sizeof(char *) * (size_lst + 2));
	if (!arr)
		return (NULL);
	fill_env_array(arr, env, &i);
	if (!my_getenv("PATH", env))
	{
		arr[i] = ft_strdup("PATH=/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:");
		i++;
	}
	arr[i] = NULL;
	return (arr);
}

char	*my_getenv(char *name, t_env *env)
{
	t_env	*temp;

	temp = env;
	while (temp)
	{
		if (ft_strcmp(temp->key, name) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return (NULL);
}

static void	handle_missing_shlvl(t_env *env)
{
	t_env	*shlvl_node;

	shlvl_node = create_node("SHLVL", "1", 1);
	append_node(&env, shlvl_node);
}

static int	calculate_new_shlvl(int shell_level)
{
	if (shell_level >= 999)
		return (0);
	else
		return (shell_level + 1);
}

void	handle_shell_level(t_env *env)
{
	int		shell_level;
	int		new_shlvl;
	int		is_valid;
	t_env	*shlvl_node;

	is_valid = 0;
	shlvl_node = get_shlvl(env);
	if (!shlvl_node || !shlvl_node->value)
	{
		handle_missing_shlvl(env);
		return ;
	}
	shell_level = ft_atoi_(shlvl_node->value, &is_valid);
	if (!is_valid)
		shell_level = 0;
	new_shlvl = calculate_new_shlvl(shell_level);
	update_node_value(shlvl_node, ft_itoa(new_shlvl), 0, 1);
}

static int	is_directory(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
		return (0);
	return (S_ISDIR(statbuf.st_mode));
}

static int	is_file(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
		return (0);
	return (S_ISREG(statbuf.st_mode));
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

static char	*check_command_path(char **allpath, char *cmd)
{
	int		i;
	char	*exec;

	i = -1;
	while (allpath && allpath[++i])
	{
		exec = build_exec_path(allpath[i], cmd);
		if (!exec)
			continue;
		if (access(exec, F_OK) == 0)
		{
			if (is_directory(exec))
			{
				free(exec);
				continue;
			}
			if (access(exec, X_OK) == 0)
			{
				free_split(allpath);
				return (exec);
			}
            else if (is_file(cmd) && access(exec, X_OK) != 0)
            {
                ft_putstr_fd("msh: ", 2);
                ft_putstr_fd(exec, 2);
                ft_putstr_fd(": Permission denied\n", 2);
                exit(126);
            }
		}
		free(exec);
	}
	return (NULL);
}

static void	print_error(char *cmd, char *msg)
{
	ft_putstr_fd("msh: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(msg, 2);
}

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
        {
            ft_putstr_fd("msh: ", 2);
            ft_putstr_fd(cmd, 2);
            ft_putstr_fd(": is a directory\n", 2);
            exit(126);
        }
        if (access(cmd, X_OK) != 0)
        {
            ft_putstr_fd("msh: ", 2);
            ft_putstr_fd(cmd, 2);
            ft_putstr_fd(": Permission denied\n", 2);
            exit(126);
        }
	return (ft_strdup(cmd));
}

// static char	*handle_no_path(char *cmd)
// {
// 	char	*current_dir;

// 	current_dir = ft_strjoin_("./", cmd);
// 	if (!current_dir)
// 	{
// 		perror("msh: ");
// 		return (NULL);
// 	}
// 	if (access(current_dir, F_OK) != 0)
// 	{
// 		print_error(cmd, ": No such file or directory\n");
// 		free(current_dir);
// 		return (NULL);
// 	}
// 	if (is_directory(current_dir))
// 	{
// 		print_error(cmd, ": Is a directory\n");
// 		free(current_dir);
// 		exit(126);
// 	}
// 	if (access(current_dir, X_OK) != 0)
// 	{
// 		print_error(cmd, ": Permission denied\n");
// 		free(current_dir);
// 		exit(126);
// 	}
// 	return (current_dir);
// }

static char	*search_in_path(char *cmd, char *path, int null_path)
{
	char	**allpath;
	char	*result;
	char	*full_path;

	allpath = ft_split(path, ':');
	if (!allpath)
	{
		perror("msh: ");
		return (NULL);
	}
	result = check_command_path(allpath, cmd);
	if (!result && null_path)
	{
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
    int     null_path;

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
		// return (handle_no_path(cmd));
	return (search_in_path(cmd, path, null_path));
}

static void	handle_minishell_exec(char *path, t_env *env)
{
	if (ft_strcmp(path, "./minishell") == 0)
		handle_shell_level(env);
}

static void	try_bash_execution(char **cmd, char *path, t_env *env)
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
		print_error(cmd[0], ": execve failed: ");
		perror("");
		free(path);
		exit(1);
	}
}

static void	handle_execve_failure(char **cmd, char *path, t_env *env)
{
	if (ft_strchr(cmd[0], '/') && is_file(path))
		try_bash_execution(cmd, path, env);
	print_error(cmd[0], ": execve failed: ");
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
		perror("open failed");
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

	i = 0;
	while (cmd->files[i].filename)
	{
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

static int	handle_pipe_error(pid_t *pid, int i)
{
	cleanup_processes(pid, i);
	perror("pipe error");
	return (1);
}

static int	handle_fork_error(pid_t *pid, int i, t_cmd *cmd, int *p_fd)
{
	cleanup_processes(pid, i);
	if (cmd->next)
	{
		close(p_fd[0]);
		close(p_fd[1]);
	}
	return (1);
}

static void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static void	setup_child_pipes(int prev_fd, t_cmd *cmd, int *p_fd)
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

static void	execute_child_command(t_cmd *cmd, t_env *env, int last_ex)
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

static int	handle_child_process(t_cmd *cmd, t_env *env, int last_ex,
			int prev_fd, int *p_fd)
{
	setup_child_signals();
	setup_child_pipes(prev_fd, cmd, p_fd);
	execute_child_command(cmd, env, last_ex);
	return (0);
}

static int	get_exit_status(int status)
{
	int	sig;
	int	last_status;

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

static void	wait_for_remaining_processes(void)
{
	while (waitpid(-1, NULL, 0) > 0)
		;
}

int	run_pipeline(t_cmd *cmd, t_env *env, int last_ex)
{
	pid_t	pid[1392];
	int		p_fd[2];
	int		prev_fd;
	int		status;
	int		i;

	prev_fd = -1;
	i = 0;
	while (cmd)
	{
		if (cmd->next && pipe(p_fd) == -1)
			return (handle_pipe_error(pid, i));
		pid[i] = fork();
		if (pid[i] == -1)
			return (handle_fork_error(pid, i, cmd, p_fd));
		if (pid[i] == 0)
			handle_child_process(cmd, env, last_ex, prev_fd, p_fd);
		if (prev_fd != -1)
			close(prev_fd);
		if (cmd->next)
		{
			close(p_fd[1]);
			prev_fd = p_fd[0];
		}
		cmd = cmd->next;
		i++;
	}
	if (prev_fd != -1)
		close(prev_fd);
	waitpid(pid[i - 1], &status, 0);
	wait_for_remaining_processes();
	return (get_exit_status(status));
}

int	run_single_cmd(t_cmd *cmd, t_env *env)
{
	pid_t	pid;
	int		status;

	status = 0;
	msh_signals();
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
		exit(1);
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