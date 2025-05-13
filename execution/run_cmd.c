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

void    run_cmd(t_cmd *cmd, t_env *env)
{
    pid_t   pid;

    if (!cmd->next)
    {
        pid = fork();
        if (pid == -1)
            return ;
        if (pid == 0)
        {
            exec(cmd->argv, env);
            exit(0);
        }
        waitpid(pid, 0, 0);
    }
    else
    {

    }
}