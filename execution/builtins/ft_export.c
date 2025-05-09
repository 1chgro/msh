#include "../../minishell.h"

void	make_index(t_env    **env)
{
	t_env	*current = NULL;
	t_env	*check = NULL;
	int		index;

	current = *env;
	index = 0;
	while (current)
	{
		check = *env;
		while (check)
		{
			if (ft_strcmp(current->key, check->key) > 0)
				index++;
			check = check->next;
		}
		current->index = index;
		index = 0;
		current = current->next;
	}
}

void    ft_export(char  **s_cmd, t_env **env)
{
    int current_index;
    int lst_size;
    t_env   *temp = NULL;
    t_env   *node;
    char    **split_space;
    char    **split_equals;
    int     i;
    // char    *str;

    current_index = 0;
    lst_size = 0;
    i = 0;
    
    make_index(env);
    temp = *env;
    if (!s_cmd[1])
    {
        while (temp)
        {
            lst_size++;
            temp = temp->next;
        }
        while (current_index < lst_size)
        {
            temp = *env;
            while (temp)
            {
                if (temp->index == current_index)
                {
                    if (temp->flag)
                        printf("declare -x %s=\"%s\"  index = %d\n", temp->key, temp->value, temp->index);
                    else
                        printf("declare -x %s  index = %d\n", temp->key, temp->index);
                    break;
                }
                temp = temp->next;
            }
            current_index++;
        }
    }
    else
    {   
        split_space = ft_split(s_cmd[1], ' ');
        if (!split_space)
            return ;
        while (split_space[i])
        {
            split_equals = ft_split(split_space[i], '=');
            if (!split_equals)
            {
                free_split(split_space);
                return ;
            }
            // if (ft_strcmp(strrchr(split_space[0], '+'), split_space[0]) != 0)

            while (temp)
            {
                if (ft_strcmp(temp->key, split_equals[0]) == 0)
                {
                    free(temp->value);
                    temp->value = split_equals[1];
                    temp->flag = 1;
                    free(split_equals[0]);
                    break;
                }
                temp = temp->next;
            }
            if (!temp)
            {
                node = create_node(split_equals);
                if (!node)
                {
                    free_split(split_space);
                    free_split(split_equals);
                    return ;
                }
                append_node(env, node);
            }
        i++;
        }
    }
}
