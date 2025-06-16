#include "../../minishell.h"

long ft_atoi_(const char *str, int *is_valid)
{
    int         i;
    long        result;
    int         sign;

    i = 0;
    result = 0;
    sign = 1;
    while (str[i] == ' ')
		{i++;}
    if (str[i] == '\0')
    {
        return (0);
    }
    if (str[i] == '-')
    {
        sign = -1;
        i++;
    }
    else if (str[i] == '+')
        {i++;}
    while (str[i] >= '0' && str[i] <= '9')
    {
        if (result > LONG_MAX)
            return (0);
        result = result * 10 + (str[i] - '0');
        i++;
    }
    while (str[i] == ' ')
       { i++;}
    if (str[i] != '\0')
    {
        return (0);
    }
    *is_valid = 1;
    return (result * sign);
}
int ft_exit(char **s_cmd)
{
    int exit_num = 0;
    int is_valid;
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
    exit_num = (int)ft_atoi_(s_cmd[1], &is_valid);
    if (!is_valid)
    {
        dup2(2, 1);
        printf("msh: exit: %s: numeric argument required\n", s_cmd[1]);
        dup2(1, 2);
        exit(255);
    }
    exit_num = exit_num % 256;
    if (exit_num < 0)
        exit_num += 256;
    exit(exit_num);
}