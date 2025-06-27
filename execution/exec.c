#include "../minishell.h"

long ft_atoi(const char *str)
{
    int i;
    long result;

    i = 0;
    result = 0;
    while (str[i] == ' ')
        i++;
    if (str[i] == '-')
    {
        i++;
    }
    else if (str[i] == '+')
    {
        i++;
    }
    while (str[i] >= '0' && str[i] <= '9')
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    while (str[i] == ' ')
        i++;
    if (str[i] != '\0')
        return (0);
    return (result);
}
int are_builtin(char *cmd)
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

static int execute_builtin(char *cmd, char **s_cmd, t_env **env, int last_ex)
{
    if (ft_strcmp(cmd, "cd") == 0)
        return (ft_cd(s_cmd, env));
    if (ft_strcmp(cmd, "echo") == 0)
        return (ft_echo(s_cmd));
    if (ft_strcmp(cmd, "pwd") == 0)
        return (ft_pwd(*env));
    if (ft_strcmp(cmd, "export") == 0)
        return (ft_export(s_cmd, env));
    if (ft_strcmp(cmd, "unset") == 0)
        return (ft_unset(s_cmd, env));
    if (ft_strcmp(cmd, "env") == 0)
        return (ft_env(s_cmd, *env));
    if (ft_strcmp(cmd, "exit") == 0)
        return (ft_exit(s_cmd, last_ex));
    return (127);
}

int run_builtin(char **s_cmd, t_env **env, int last_ex)
{
    if (!s_cmd || !s_cmd[0])
    {
        perror("invalid built-in command");
        return (1);
    }
    return (execute_builtin(s_cmd[0], s_cmd, env, last_ex));
}

static int process_heredoc_file(t_red *file, t_glob_st *glob_strct)
{
    int status;

    status = here_doc(file->filename, &file->fd, &file->expand_flg, glob_strct);
    if (status)
        return (status);
    if (file->fd < 0)
    {
        perror("msh : ");
        return (status);
    }
    return (0);
}

static int process_cmd_heredocs(t_cmd *cmd, t_glob_st *glob_strct)
{
    int i;
    int status;

    if (!cmd->files)
        return (0);
    i = 0;
    while (cmd->files[i].filename)
    {
        if (cmd->files[i].type == HEREDOC)
        {
            status = process_heredoc_file(&cmd->files[i], glob_strct);
            if (status)
                return (status);
        }
        i++;
    }
    return (0);
}

int open_heredoc(t_glob_st *glob_strct)
{
    t_cmd *temp;
    int status;

    temp = glob_strct->cmd;
    status = 0;
    while (temp)
    {
        status = process_cmd_heredocs(temp, glob_strct);
        if (status)
            return (status);
        temp = temp->next;
    }
    return (status);
}

static void close_cmd_heredocs(t_cmd *cmd)
{
    int i;

    if (!cmd->files)
        return;
    i = 0;
    while (cmd->files[i].filename)
    {
        if (cmd->files[i].type == HEREDOC && cmd->files[i].fd >= 0)
            close(cmd->files[i].fd);
        i++;
    }
}

void close_heredoc(t_glob_st *glob_strct)
{
    t_cmd *temp;

    temp = glob_strct->cmd;
    while (temp)
    {
        close_cmd_heredocs(temp);
        temp = temp->next;
    }
}

static int count_commands(t_cmd *cmd)
{
    int size;
    t_cmd *temp;

    size = 0;
    temp = cmd;
    while (temp)
    {
        size++;
        temp = temp->next;
    }
    return (size);
}

static int save_std_fds(int *saved_stdout, int *saved_stdin)
{
    *saved_stdout = dup(STDOUT_FILENO);
    *saved_stdin = dup(STDIN_FILENO);
    if (*saved_stdout < 0 || *saved_stdin < 0)
    {
        perror("dup error");
        return (1);
    }
    return (0);
}

static void restore_std_fds(int saved_stdout, int saved_stdin)
{
    dup2(saved_stdout, STDOUT_FILENO);
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdout);
    close(saved_stdin);
}

static int handle_redirection_only(t_glob_st *glob_strct, int saved_stdout, int saved_stdin)
{
    int status;

    status = redirection(glob_strct->cmd);
    if (status != 0)
    {
        glob_strct->ext_stat = status;
        restore_std_fds(saved_stdout, saved_stdin);
        return (status);
    }
    return (0);
}

static int handle_builtin_command(t_glob_st *glob_strct, int saved_stdout, int saved_stdin)
{
    int status;

    if (glob_strct->cmd->files)
    {
        status = redirection(glob_strct->cmd);
        if (status != 0)
        {
            glob_strct->ext_stat = status;
            restore_std_fds(saved_stdout, saved_stdin);
            return (status);
        }
    }
    glob_strct->ext_stat = run_builtin(glob_strct->cmd->argv, &glob_strct->env, glob_strct->ext_stat);
    return (0);
}

int msh_execute(t_glob_st *glob_strct)
{
    int size;
    int saved_stdout;
    int saved_stdin;

    if (!glob_strct->cmd)
        return (1);
    size = count_commands(glob_strct->cmd);
    if (save_std_fds(&saved_stdout, &saved_stdin))
        return (1);
    if (!glob_strct->cmd->argv && size == 1)
    {
        if (glob_strct->cmd->files)
        {
            if (handle_redirection_only(glob_strct, saved_stdout, saved_stdin))
                return (glob_strct->ext_stat);
        }
    }
    else if (glob_strct->cmd->argv && are_builtin(glob_strct->cmd->argv[0]) && size == 1)
    {
        if (handle_builtin_command(glob_strct, saved_stdout, saved_stdin))
            return (glob_strct->ext_stat);
    }
    else
        glob_strct->ext_stat = run_cmd(glob_strct->cmd, glob_strct->env, glob_strct->ext_stat);
    return (restore_std_fds(saved_stdout, saved_stdin), close_heredoc(glob_strct), glob_strct->ext_stat);
}
