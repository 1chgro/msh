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
    ft_putstr_fd("msh: ", 2);
    ft_putstr_fd(s_cmd[0], 2);
    ft_putendl_fd(": command not found", 2);
    return (127);
}

// int    msh_execute(t_cmd *cmd, t_env *env)
// {
//     int     status = 0;
//     int size = 0;
//     t_cmd   *temp = cmd;
//     int i;

//     if (!cmd)
//     {
//         return (1);
//     }
//     while (temp)
//     {
//         size++;
//         temp = temp->next;
//     }
//     temp = cmd;
//     while (temp)
//     {
//         if (temp->files)
//         {
//             i = 0;
//             while (temp->files[i].filename)
//             {
//                 if (temp->files[i].type == HEREDOC)
//                 {
//                     status = here_doc(temp->files[i].filename, &temp->files[i].fd, env);
//                     if (temp->files[i].fd < 0)
//                         return (1);
//                 }
//                 i++;
//             }
//         }
//         temp = temp->next;
//     }
//     if (!cmd->argv)
//     {
//         int saved_stdout = dup(STDOUT_FILENO);
//         int saved_stdin = dup(STDIN_FILENO);
//         if (cmd->files)
//         {
//             char *line;
//             status = redirection(cmd);
//             while ((line = get_next_line(STDIN_FILENO)))
//             {
//                 write(STDOUT_FILENO, line, ft_strlen(line));
//                 free(line);
//             }
//         }
//         dup2(saved_stdout, STDOUT_FILENO);
//         dup2(saved_stdin, STDIN_FILENO);
//         close(saved_stdout);
//         close(saved_stdin);

//     }
//     else if (are_builtin(cmd->argv[0]) && size == 1)
//     {
//         int saved_stdout = dup(STDOUT_FILENO);
//         int saved_stdin = dup(STDIN_FILENO);
//         if (cmd->files)
//         {
//             status = redirection(cmd);
//         }
//         status = run_builtin(cmd->argv, env);
//         dup2(saved_stdout, STDOUT_FILENO);
//         dup2(saved_stdin, STDIN_FILENO);
//         close(saved_stdout);
//         close(saved_stdin);
//     }
//     else
//     {
//         status = run_cmd(cmd, env);
//     }
//     temp = cmd;
//     while (temp)
//     {
//         if (temp->files)
//         {
//             int i = 0;
//             while (temp->files[i].filename)
//             {
//                 if (temp->files[i].type == HEREDOC && temp->files[i].fd >= 0)
//                     close(temp->files[i].fd);
//                 i++;
//             }
//         }
//         temp = temp->next;
//     }
//     return (status);
// }

int msh_execute(t_cmd *cmd, t_env *env)
{
    int status = 0;
    int size = 0;
    t_cmd *temp = cmd;
    int i;
    int saved_stdout = -1;
    int saved_stdin = -1;

    if (!cmd)
        return (1);

    while (temp)
    {
        size++;
        temp = temp->next;
    }
    temp = cmd;
    while (temp)
    {
        if (temp->files)
        {
            i = 0;
            while (temp->files[i].filename)
            {
                if (temp->files[i].type == HEREDOC)
                {
                    status = here_doc(temp->files[i].filename, &temp->files[i].fd, env);
                    if (temp->files[i].fd < 0)
                    {
                        perror("heredoc error");
                        return (1);
                    }
                }
                i++;
            }
        }
        temp = temp->next;
    }
    saved_stdout = dup(STDOUT_FILENO);
    saved_stdin = dup(STDIN_FILENO);
    if (saved_stdout < 0 || saved_stdin < 0)
    {
        perror("dup error");
        return (1);
    }

    if (!cmd->argv)
    {
        if (cmd->files)
        {
            status = redirection(cmd);
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
    else if (are_builtin(cmd->argv[0]) && size == 1)
    {
        if (cmd->files)
        {
            status = redirection(cmd);
            if (status != 0)
            {
                dup2(saved_stdout, STDOUT_FILENO);
                dup2(saved_stdin, STDIN_FILENO);
                close(saved_stdout);
                close(saved_stdin);
                return (status);
            }
        }
        status = run_builtin(cmd->argv, env);
    }
    else
    {
        status = run_cmd(cmd, env);
    }
    dup2(saved_stdout, STDOUT_FILENO);
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdout);
    close(saved_stdin);
    temp = cmd;
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

    return (status);
}