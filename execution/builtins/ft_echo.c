#include "../../minishell.h"

int check_newline(char  *str)
{
    int i;

    i = 0;
    if (str[i++] == '-')
    {
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

    i = 1;
    new_line = check_newline(args[1]);
    if (new_line)
    {
        while (args[i])
        {
            printf("%s", args[i]);
            if (args[i + 1])
                printf(" ");
            i++;
        }
        printf("\n");
    }
    else
    {
        i = 2;
        while (args[i])
        {
            printf("%s", args[i]);
            if (args[i + 1])
                printf(" ");
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