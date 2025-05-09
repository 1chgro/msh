#include "../../minishell.h"
void    ft_exit(char **s_cmd)
{
    int exit_num;

    exit_num = 0;
    if (!s_cmd[1])
    {
        printf("exit\n");
        exit(0);
    }
    else
    {
        exit_num = ft_atoi(s_cmd[1]);
        if (!exit_num)
        {
            printf("exit\n");
            printf("msh: exit: : numeric argument required \n");
            exit(255);
        }
        else
        {
            if (exit_num > 256)
            {
                exit_num = exit_num % 256;
                printf("exit\n");
                exit(exit_num);
            }
            else if (exit_num < 0)
            {
                printf("exit\n");
                exit(exit_num + 256);
            }
            else
            {
                printf("exit\n");
                exit(exit_num);  
            }
        }
    }
}
