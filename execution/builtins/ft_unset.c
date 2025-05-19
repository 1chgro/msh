#include "../../minishell.h"

static void remove_variable(char *key, t_env **env)
{
    t_env *current = *env;
    t_env *prev = NULL;
    if (!current)
        return;
    if (ft_strcmp(current->key, key) == 0)
    {
        *env = current->next;
        free(current->key);
        free(current->value);
        free(current);
        return;
    }
    while (current && ft_strcmp(current->key, key) != 0)
    {
        prev = current;
        current = current->next;
    }
    if (!current)
        return;
    prev->next = current->next;
    free(current->key);
    free(current->value);
    free(current);
}
void    ft_unset(char **s_cmd, t_env **env)
{
    t_env   *temp;
    int     flag;
    int     i;

    flag = 0;
    i = 0;
    if (!s_cmd[1])
        return ;
    while (s_cmd[i])
    {
        temp = *env;
        while(temp)
        {   
            if (ft_strcmp(s_cmd[i], temp->key) == 0)
            {
                flag = 1;
                break ;
            }
            temp = temp->next;
        }
        if (flag == 1)
            remove_variable(s_cmd[i], env);
        i++;
    }
}