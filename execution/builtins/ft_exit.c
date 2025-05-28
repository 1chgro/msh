#include "../../minishell.h"

int ft_exit(char **s_cmd)
{
    int exit_num = 0;

    if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
        write(STDERR_FILENO, "exit\n", 5);
    if (!s_cmd[1])
    {
        exit(0);
    }
    if (s_cmd[2])
    {
        dup2(2, 1);
        printf("bash: exit: too many arguments\n");
        dup2(1, 2);
        return(1);
    }
    exit_num = (int)ft_atoi(s_cmd[1]);
    if (ft_atoi(s_cmd[1]) == 0 && s_cmd[1][0] != '0')
    {
        dup2(2, 1);
        printf("msh: exit: %s: numeric argument required\n", s_cmd[1]);
        dup2(1, 2);
        exit(2);
    }
    exit_num = exit_num % 256;
    if (exit_num < 0)
        exit_num += 256;
    exit(exit_num);
}