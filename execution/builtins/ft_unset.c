#include "../../minishell.h"

static void remove_variable(char *key, t_env **env)
{
    t_env *current = *env;
    t_env *prev = NULL;
    // t_env *tofree;

    // Empty list
    if (!current)
        return;

    // Check head node
    if (ft_strcmp(current->key, key) == 0)
    {
        *env = current->next;
        free(current->key);
        free(current->value);
        free(current);
        return;
    }

    // Search for node
    while (current && ft_strcmp(current->key, key) != 0)
    {
        prev = current;
        current = current->next;
    }

    // Node not found
    if (!current)
        return;

    // Remove node
    prev->next = current->next;
    free(current->key);
    free(current->value);
    free(current);
}
void    ft_unset(char **s_cmd, t_env **env)
{
    t_env   *temp;
    int     flag;
    char    **split_args;
    int     i;

    flag = 0;
    i = 0;
    if (!s_cmd[1])
        return ;
    split_args = ft_split(s_cmd[1], ' ');
    while (split_args[i])
    {
        temp = *env;
        while(temp)
        {   
            if (ft_strcmp(split_args[i], temp->key) == 0)
            {
                flag = 1;
                break ;
            }
            temp = temp->next;
        }
        if (flag == 1)
            remove_variable(split_args[i], env);
        i++;
    }
}