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
        {return (0);}
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
void	run_builtin(char **s_cmd, t_env *env)
{
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
}

int    msh_execute(t_cmd *cmd, t_env *env)
{
    int size = 0;
    t_cmd   *temp = cmd;

    if (!cmd)
        {return (0);}
    while (temp)
    {
        size++;
        temp = temp->next;
    }
    if (!cmd->argv)
    {
        int saved_stdout = dup(STDOUT_FILENO);
        int saved_stdin = dup(STDIN_FILENO);
        if (cmd->files)
            redirection(cmd);
        dup2(saved_stdout, STDOUT_FILENO);
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdout);
        close(saved_stdin);

    }
    else if (are_builtin(cmd->argv[0]) && size == 1)
    {
        int saved_stdout = dup(STDOUT_FILENO);
        int saved_stdin = dup(STDIN_FILENO);
        if (cmd->files)
        {
            redirection(cmd);
        }
        run_builtin(cmd->argv, env);
        dup2(saved_stdout, STDOUT_FILENO);
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdout);
        close(saved_stdin);
    }
    else
    {
        run_cmd(cmd, env);
    }
    return (1);
}