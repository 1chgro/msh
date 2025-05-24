#include "../../minishell.h"
// int    ft_exit(char **s_cmd)
// {
//     int exit_num;

//     exit_num = 0;
//     if (!s_cmd[1])
//     {
//         if (isatty(STDOUT_FILENO))
//             printf("exit\n");
//         exit(0);
//     }
//     else
//     {
//         exit_num = ft_atoi(s_cmd[1]);
//         if (!exit_num)
//         {
//             if (isatty(STDOUT_FILENO))
//             {
//                 printf("exit\n");
//                 printf("msh: exit: : numeric argument required \n");
//             }
//             exit(255);
//         }
//         else
//         {
//             if (exit_num > 256)
//             {
//                 exit_num = exit_num % 256;
//                 if (isatty(STDOUT_FILENO))
//                     printf("exit\n");
//                 exit(exit_num);
//             }
//             else if (exit_num < 0)
//             {
//                 if (isatty(STDOUT_FILENO))
//                     printf("exit\n");
//                 exit(exit_num + 256);
//             }
//             else
//             {
//                 if (isatty(STDOUT_FILENO))
//                     printf("exit\n");
//                 exit(exit_num);  
//             }
//         }
//     }
//     return (0);
// }
int ft_exit(char **s_cmd)
{
    int exit_num = 0;
    int is_interactive = isatty(STDIN_FILENO);

    if (is_interactive)
        write(STDERR_FILENO, "exit\n", 5);
    if (!s_cmd[1])
    {
        exit(0);
    }
    exit_num = (int)ft_atoi(s_cmd[1]);
    if (ft_atoi(s_cmd[1]) == 0 && s_cmd[1][0] != '0')
    {
        ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
        ft_putstr_fd(s_cmd[1], STDERR_FILENO);
        ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
        exit(2);
    }
    exit_num = exit_num % 256;
    if (exit_num < 0)
        exit_num += 256;
    exit(exit_num);
}