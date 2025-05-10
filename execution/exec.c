#include "../minishell.h"

long ft_atoi(const char *str)
{
    int         i;
    long        result;
    int         sign;

    i = 0;
    result = 0;
    sign = 1;
	if (str[i] == '\"')
		i++;
    while (str[i] == ' ')
		i++;
    if (str[i] == '-')
    {
        sign = -1;
        i++;
    }
    else if (str[i] == '+')
        i++;
    while (str[i] >= '0' && str[i] <= '9')
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    while (str[i] == ' ')
        i++;
	if (str[i] == '\"')
	i++;
    if (str[i] != '\0')
        return (0);
    return (result * sign);
}
static int  are_builtin(char    *cmd)
{
    if (!cmd)
        return (0);
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
static void	run_builtin(char **s_cmd, t_env *env)
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
    if (!cmd)
        return (0);
	while (cmd)
    {
        if (are_builtin(cmd->argv[0]))
        {
            run_builtin(cmd->argv, env);
        }
        else
        {
            run_cmd(cmd, env);
        }
        cmd = cmd->next;
    }
    return (1);
}