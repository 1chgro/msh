#include "../../minishell.h"

void    ft_pwd()
{
    char    buff[8192000];

    getcwd(buff, 8192000);
    printf("%s\n", buff);
}
