#include "../../minishell.h"

int check_newline(char  *str)
{
    int i;

    i = 0;
    if (str[i] == '-' && str[i + 1])
    {
        i++;
        while (str[i])
        {
            if (str[i] != 'n')
                return (1);
            i++;
        }
    }
    else
        return (1);
    return (0);
}

void    ft_handle_echo_arg(char **args)
{
    int     i;
    int     new_line;

    i = 0;
    new_line = check_newline(args[1]);
    if (new_line)
    {
        i++;
        while (args[i])
        {
            ft_putstr_fd(args[i], 1);
            if (args[i + 1])
                ft_putstr_fd(" ", 1);
            i++;
        }
        printf("\n");
    }
    else
    {
        i = 1;
        while (args[i] && !check_newline(args[i]))
            i++;
        while (args[i])
        {
            ft_putstr_fd(args[i], 1);
            if (args[i + 1])
                ft_putstr_fd(" ", 1);
            i++;
        }
    }
}

int    ft_echo(char    **s_cmd)
{
    if (!s_cmd[1])
    {
        printf("\n");
    }
    else
    {
        ft_handle_echo_arg(s_cmd);
    }
    return(0);
}