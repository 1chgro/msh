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
t_env    *get_shlvl(t_env *env)
{
	t_env   *temp;

	temp = env;
	while (temp)
	{
		if (ft_strcmp(temp->key, "SHLVL") == 0)
			return (temp);
		temp =temp->next;
	}
	return (NULL);
}

char    **struct_to_array(t_env *env)
{
	int i;
	int size_lst;
	char    **arr;
	char    *str;
	t_env   *temp = NULL;

	i = 0;
	size_lst = 0;
	temp = env;
	while (temp)
	{
		size_lst++;
		temp = temp->next;
	}
	temp = env;
	arr = malloc(sizeof(char *) * (size_lst + 2));
	if (!arr)
		return  (NULL);
	while (temp)
	{
		str = ft_strjoin_(temp->key, "=");
		arr[i++] = ft_strjoin_(str, temp->value);
		temp = temp->next;
	}
    if (!my_getenv("PATH", env))
    {
        arr[i] = ft_strdup("PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
        i++;
    }
	arr[i] = NULL;
	return (arr);
}

char	*my_getenv(char *name, t_env *env)
{
	t_env   *temp = NULL;

	temp = env;
	while (temp)
	{
		if (ft_strcmp(temp->key, name) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return (NULL);
}

void    handle_shell_level(t_env *env)
{
	int shell_level;
	int new_shlvl;
    int is_valid = 0;
	t_env *shlvl_node = get_shlvl(env);

	if (!shlvl_node || !shlvl_node->value)
	{
		shlvl_node = create_node("SHLVL", "1", 1);
		append_node(&env, shlvl_node);
		return ;
	}
		shell_level = ft_atoi_(shlvl_node->value, &is_valid);
    if (!is_valid)
    {
        shell_level = 0;
    }
	if (shell_level >= 999)
	{
		new_shlvl = 1;
	} else
	{
		new_shlvl = shell_level + 1;
	}
	update_node_value(shlvl_node, ft_itoa(new_shlvl), 0, 1);
}

static int is_directory(const char *path)
{
	struct stat statbuf;
	if (stat(path, &statbuf) != 0)
		return 0;
	return S_ISDIR(statbuf.st_mode);
}
static int is_file(const char *path)
{
	struct stat statbuf;
	if (stat(path, &statbuf) != 0)
		return 0;
	return S_ISREG(statbuf.st_mode);
}

static char *check_command_path(char **allpath, char *cmd)
{
	int i = -1;
	char *path_part;
	char *exec;

	while (allpath && allpath[++i])
	{
		path_part = ft_strjoin_(allpath[i], "/");
		if (!path_part)
			continue;
		exec = ft_strjoin_(path_part, cmd);
		free(path_part);
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
				return exec;
			}
		}
		free(exec);
	}
	return NULL;
}


char *get_path(char *cmd, t_env *env)
{
	char **allpath;
	char *result;

	if (!cmd)
		return NULL;
	if (!env)
	{
		ft_putstr_fd("msh: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return NULL;
	}
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) != 0)
		{
			ft_putstr_fd("msh: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			return NULL;
		}
		if (is_directory(cmd))
		{
			ft_putstr_fd("msh: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(": Is a directory\n", 2);
			exit(126);
		}
		if (access(cmd, X_OK) != 0)
		{
			ft_putstr_fd("msh: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putstr_fd(": Permission denied\n", 2);
			return NULL;
		}
		return ft_strdup(cmd);
	}
	char *path = my_getenv("PATH", env);
	if (!path)
	{
		path = ft_strdup(":.");
	}
	allpath = ft_split(path, ':');
	if (!allpath)
	{
		ft_putstr_fd("msh: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		return NULL;
	}
	result = check_command_path(allpath, cmd);
	// free_split(allpath);
	if (!result)
	{
		ft_putstr_fd("msh: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	return result;
}

void exec(char **cmd, t_env *env)
{
	char *path;

	if (!cmd || !cmd[0])
		exit(1);
	path = get_path(cmd[0], env);
	if (!path)
		exit(127);
	if (ft_strcmp(path, "./minishell") == 0)
	{
		handle_shell_level(env);
	}
    if (ft_strchr(cmd[0], '/') && is_file(path))
    {
        char **new_arg = malloc((sizeof(char *) * 3));
        if (!new_arg)
        {
            perror("malloc");
        }
        new_arg[0] = ft_strdup("bash");
        new_arg[1] = path;
        new_arg[2] = NULL;
        if (execve("/bin/bash", new_arg, struct_to_array(env)) == -1)
        {
                ft_putstr_fd("msh: ", 2);
                ft_putstr_fd(cmd[0], 2);
                ft_putstr_fd(": execve failed: ", 2);
                perror("");
                free(path);
                exit(1);
        }
    }
    else
    {
        if (execve(path, cmd, struct_to_array(env)) == -1)
        {
                ft_putstr_fd("msh: ", 2);
                ft_putstr_fd(cmd[0], 2);
                ft_putstr_fd(": execve failed: ", 2);
                perror("");
                free(path);
                exit(1);
        }
    }
}

int redirection(t_cmd *cmd)
{
	int i = 0;
	while (cmd->files[i].filename)
	{
		if (cmd->files[i].type == HEREDOC)
		{
			if (cmd->files[i].fd < 0 || dup2(cmd->files[i].fd, STDIN_FILENO) < 0)
			{
				perror("dup2 or heredoc failed");
				if (cmd->files[i].fd >= 0)
					close(cmd->files[i].fd);
				return (1);
			}
		}
		else if (cmd->files[i].type == REDIRECT_IN)
		{
			cmd->files[i].fd = open(cmd->files[i].filename, O_RDONLY);
			if (cmd->files[i].fd < 0)
			{
				perror("open failed");
				return (1);
			}
			dup2(cmd->files[i].fd, STDIN_FILENO);
			close(cmd->files[i].fd);
		}
		else if (cmd->files[i].type == REDIRECT_OUT)
		{
			if (ft_strcmp(cmd->files[i].filename, "/dev/stdout") == 0)
				return (0);
			cmd->files[i].fd = open(cmd->files[i].filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
			if (cmd->files[i].fd < 0)
			{
				perror("open failed");
				return (1);
			}
			dup2(cmd->files[i].fd, STDOUT_FILENO);
			close(cmd->files[i].fd);
		}
		else if (cmd->files[i].type == APPEND)
		{
			cmd->files[i].fd = open(cmd->files[i].filename, O_CREAT | O_RDWR | O_APPEND, 0644);
			if (cmd->files[i].fd < 0)
			{
				perror("open failed");
				return (1);
			}
			dup2(cmd->files[i].fd, STDOUT_FILENO);
			close(cmd->files[i].fd);
		}
		i++;
	}
	return (0);
}

int run_pipeline(t_cmd *cmd, t_env *env, int last_ex)
{
	pid_t pid[1392];
	int p_fd[2];
	int prev_fd = -1;
	int     status;
	int i = 0;
	int j = 0;
	int	sig;
	int     last_status = 0;

	while (cmd)
	{
		if (cmd->next)
		{
			if (pipe(p_fd) == -1)
			{
				j = 0;
				while (j < i)
				{
					kill(pid[j], SIGKILL);
					waitpid(pid[j], NULL, 0);
					j++;
				}
				perror("pipe error");
				return (1);
			}
		}
		pid[i] = fork();
		
		if (pid[i] == -1)
		{
			j = 0;
			while (j < i)
			{
				if (kill(pid[j], SIGKILL) == -1)
					perror("Failed to kill pid");
				waitpid(pid[j], NULL, 0);
				j++;
			}
			if (cmd->next)
			{
				close(p_fd[0]);
				close(p_fd[1]);
			}
			return (1);
		}
		if (pid[i] == 0)
		{
			signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
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
		}
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

	while (waitpid(-1, &status, 0) > 0)
	{
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
	}
	return (last_status);
}

int    run_single_cmd(t_cmd *cmd, t_env *env)
{
	pid_t pid;
	int status = 0;
	int sig;

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
		signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
		if (cmd->files)
		{
			status = redirection(cmd);
			if (status != 0)
				exit(status);
		}
		exec(cmd->argv, env);
		exit(1);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			status = SIGINT + 128;
		else if (sig == SIGQUIT)
		{
			dup2(2, 1);
			printf("Quit: %d\n", SIGQUIT);
			dup2(1, 2);
			status = SIGQUIT + 128;
		}
	}	return (status);
}

int run_cmd(t_cmd *cmd, t_env *env, int last_ex)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
	{
		perror("invalid command");
		return (1);
	}
	if (!cmd->next)
	{
		return (run_single_cmd(cmd, env));
	}
	return (run_pipeline(cmd, env, last_ex));
}