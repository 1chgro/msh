#include "../../minishell.h"
void    ft_exit(char **s_cmd)
{
    int exit_num;

    exit_num = 0;
    if (!s_cmd[1])
    {
        if (isatty(STDOUT_FILENO))
            printf("exit\n");
        exit(0);
    }
    else
    {
        exit_num = ft_atoi(s_cmd[1]);
        if (!exit_num)
        {
            if (isatty(STDOUT_FILENO))
            {
                printf("exit\n");
                printf("msh: exit: : numeric argument required \n");
            }
            exit(255);
        }
        else
        {
            if (exit_num > 256)
            {
                exit_num = exit_num % 256;
                if (isatty(STDOUT_FILENO))
                    printf("exit\n");
                exit(exit_num);
            }
            else if (exit_num < 0)
            {
                if (isatty(STDOUT_FILENO))
                    printf("exit\n");
                exit(exit_num + 256);
            }
            else
            {
                if (isatty(STDOUT_FILENO))
                    printf("exit\n");
                exit(exit_num);  
            }
        }
    }
}
