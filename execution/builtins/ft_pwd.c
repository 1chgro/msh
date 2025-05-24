#include "../../minishell.h"

int    ft_pwd()
{
    char    buff[8192000];

    getcwd(buff, 8192000);
    printf("%s\n", buff);
    return (0);
}
