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
    arr = malloc(sizeof(char *) * (size_lst + 1));
    if (!arr)
        return  (NULL);
    while (temp)
    {
        str = ft_strjoin_(temp->key, "=");
        arr[i++] = ft_strjoin_(str, temp->value);
        temp = temp->next;
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

static char	*check_command_path(char **allpath, char *cmd)
{
	int		i;
	char	*path_part;
	char	*exec;

	i = -1;
	while (allpath[++i])
	{
		path_part = ft_strjoin_(allpath[i], "/");
		exec = ft_strjoin_(path_part, cmd);
		free(path_part);
		if (access(exec, F_OK | X_OK) == 0)
		{
			free_split(allpath);
			return (exec);
		}
		free(exec);
	}
	return (NULL);
}

char	*get_path(char *cmd, t_env *env)
{
	char	**allpath;
	char	*result;

	allpath = ft_split(my_getenv("PATH", env), ':');

	if (!allpath || !cmd)
	{
		free_split(allpath);
		return (NULL);
	}
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (cmd);
		return (NULL);
	}
	result = check_command_path(allpath, cmd);
	if (result)
		return (result);
	free_split(allpath);
	return (NULL);
}

void	exec(char **cmd, t_env *env)
{
	char	*path;
    if (!cmd)
        {return ;}
	path = get_path(cmd[0], env);
	if (!path)
	{
		perror("msh: command not found: ");
		perror(cmd[0]);
		exit(127);
	}
	if (execve(path, cmd, struct_to_array(env)) == -1)
	{
		free(path);
		exit(1);
	}
}
void redirection(t_cmd *cmd)
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
                exit(1);
            }
		}
        else if (cmd->files[i].type == REDIRECT_IN)
        {
            cmd->files[i].fd = open(cmd->files[i].filename, O_RDONLY);
            if (cmd->files[i].fd < 0)
            {
                perror("open failed");
                exit(1);
            }
            dup2(cmd->files[i].fd, STDIN_FILENO);
            close(cmd->files[i].fd);
        }
        else if (cmd->files[i].type == REDIRECT_OUT)
        {
            cmd->files[i].fd = open(cmd->files[i].filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
            if (cmd->files[i].fd < 0)
            {
                perror("open failed");
                exit(1);
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
                exit(1);
            }
            dup2(cmd->files[i].fd, STDOUT_FILENO);
            close(cmd->files[i].fd);
        }
        i++;
    }
}

void run_pipeline(t_cmd *cmd, t_env *env)
{
    pid_t pid;
    int p_fd[2];
    int prev_fd = -1;

    while (cmd)
    {
        if (cmd->next)
        {
            if (pipe(p_fd) == -1)
            {
                perror("pipe error");
                break;
            }
        }
        pid = fork();
        if (pid == -1)
        {
            perror("fork error");
            if (cmd->next)
            {
                close(p_fd[0]);
                close(p_fd[1]);
            }
            break;
        }
        if (pid == 0)
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
            if (cmd->files)
                redirection(cmd);
            if (are_builtin(cmd->argv[0]))
            {
                run_builtin(cmd->argv, env);
                exit(0);
            }
            else
            {
                exec(cmd->argv, env);
                exit(1);
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
    }
    if (prev_fd != -1)
        close(prev_fd);
    while (wait(NULL) > 0)
        ;
}

void    run_single_cmd(t_cmd *cmd, t_env *env)
{
    pid_t pid;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return ;
    }
    if (pid == 0)
    {
        if (cmd->files)
            redirection(cmd);
        exec(cmd->argv, env);
        exit(1);
    }
    waitpid(pid, 0, 0);
}

void    run_cmd(t_cmd *cmd, t_env *env)
{
    if (!cmd)
    {
        return ;
    }
    if (!cmd->next)
    {
        run_single_cmd(cmd, env);
    }
    else
        run_pipeline(cmd, env);
}