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

void    ft_handle_echo_arg(char *args)
{
    char    **splited;
    int     i;
    int     new_line;

    i = 0;
    splited = ft_split(args, ' ');
    if (!splited)
        return ;
    new_line = check_newline(splited[0]);
    if (new_line)
    {
        while (splited[i])
        {
            printf("%s", splited[i]);
            if (splited[i + 1])
                printf(" ");
            i++;
        }
        printf("\n");
    }
    else
    {
        i = 1;
        while (splited[i])
        {
            printf("%s", splited[i]);
            if (splited[i + 1])
                printf(" ");
            i++;
        }
    }
    free_split(splited);
}

void    ft_echo(char    **s_cmd)
{
    if (!s_cmd[1])
    {
        printf("\n");
    }
    else
    {
        ft_handle_echo_arg(s_cmd[1]);
    }
}