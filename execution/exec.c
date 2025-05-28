#include "../minishell.h"

long ft_atoi(const char *str)
{
    int         i;
    long        result;
    int         sign;

    i = 0;
    result = 0;
    sign = 1;
    while (str[i] == ' ')
		{i++;}
    if (str[i] == '-')
    {
        sign = -1;
        i++;
    }
    else if (str[i] == '+')
        {i++;}
    while (str[i] >= '0' && str[i] <= '9')
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    while (str[i] == ' ')
       { i++;}
    if (str[i] != '\0')
    {
        return (0);
    }
    return (result * sign);
}
int  are_builtin(char    *cmd)
{
    if (!cmd)
        {return (0);}
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}
int	run_builtin(char **s_cmd, t_env *env)
{
    if (!s_cmd || !s_cmd[0])
    {
        perror("invalid built-in command");
        return (1);
    }
	if (ft_strcmp(s_cmd[0], "cd") == 0)
		return (ft_cd(s_cmd, &env));
	if (ft_strcmp(s_cmd[0], "echo") == 0)
		return (ft_echo(s_cmd));
	if (ft_strcmp(s_cmd[0], "pwd") == 0)
		return (ft_pwd(s_cmd, env));
	if (ft_strcmp(s_cmd[0], "export") == 0)
		return (ft_export(s_cmd, &env));
	if (ft_strcmp(s_cmd[0], "unset") == 0)
		return (ft_unset(s_cmd, &env));
	if (ft_strcmp(s_cmd[0], "env") == 0)
		return (ft_env(s_cmd, env));
	if (ft_strcmp(s_cmd[0], "exit") == 0)
		return (ft_exit(s_cmd));
    return (127);
}

int open_heredoc(t_glob_st *glob_strct)
{
    t_cmd *temp = glob_strct->cmd;
    int i;
    int status = 0;
    temp = glob_strct->cmd;
    while (temp)
    {
        if (temp->files)
        {
            i = 0;
            while (temp->files[i].filename)
            {
                if (temp->files[i].type == HEREDOC)
                {
                    status = here_doc(temp->files[i].filename, &temp->files[i].fd, glob_strct);
                    if (status)
                        return (status);
                    if (temp->files[i].fd < 0)
                    {
                        perror("msh : ");
                        return (status);
                    }
                }
                i++;
            }
        }
        temp = temp->next;
    }
    return(status);
}
void close_heredoc(t_glob_st *glob_strct)
{
    t_cmd   *temp;
    int i;

    temp = glob_strct->cmd;
    while (temp)
    {
        if (temp->files)
        {
            i = 0;
            while (temp->files[i].filename)
            {
                if (temp->files[i].type == HEREDOC && temp->files[i].fd >= 0)
                    close(temp->files[i].fd);
                i++;
            }
        }
        temp = temp->next;
    }
}
int msh_execute(t_glob_st *glob_strct)
{
    int status = 0;
    int size = 0;
    t_cmd *temp = glob_strct->cmd;
    int saved_stdout = -1;
    int saved_stdin = -1;

    if (!glob_strct->cmd)
        return (1);

    while (temp)
    {
        size++;
        temp = temp->next;
    }
    saved_stdout = dup(STDOUT_FILENO);
    saved_stdin = dup(STDIN_FILENO);
    if (saved_stdout < 0 || saved_stdin < 0)
    {
        perror("dup error");
        return (1);
    }
    if (!glob_strct->cmd->argv)
    {
        if (glob_strct->cmd->files)
        {
            status = redirection(glob_strct->cmd);
            if (status != 0)
            {
                dup2(saved_stdout, STDOUT_FILENO);
                dup2(saved_stdin, STDIN_FILENO);
                close(saved_stdout);
                close(saved_stdin);
                return (status);
            }
        }
    }
    else if (are_builtin(glob_strct->cmd->argv[0]) && size == 1)
    {
        if (glob_strct->cmd->files)
        {
            status = redirection(glob_strct->cmd);
            if (status != 0)
            {
                dup2(saved_stdout, STDOUT_FILENO);
                dup2(saved_stdin, STDIN_FILENO);
                close(saved_stdout);
                close(saved_stdin);
                return (status);
            }
        }
        glob_strct->ext_stat = run_builtin(glob_strct->cmd->argv, glob_strct->env);
    }
    else
    {
        glob_strct->ext_stat = run_cmd(glob_strct->cmd, glob_strct->env);
    }
    dup2(saved_stdout, STDOUT_FILENO);
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdout);
    close(saved_stdin);
    close_heredoc(glob_strct);
    return (glob_strct->ext_stat);
}