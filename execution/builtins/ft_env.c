#include "../../minishell.h"

int    ft_env(char **s_cmd, t_env *env)
{
    t_env *temp = NULL;

    if (!env)
        return (0);
    temp = env;
    (void)s_cmd;
    while (temp)
    {
        if (temp->flag == 1)
            printf("%s=%s\n", temp->key, temp->value);
        temp = temp->next;
    }
    return (0);
}