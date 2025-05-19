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
    char    *key;
    char    *value;
    int     i;

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
        i = 1;
        while (s_cmd[i])
        {
            key = get_key(s_cmd[i]);
            if (!key)
            {
                return;
            }
            value = get_value(s_cmd[i]);
            if (!value)
            {
                free(key);
                return;
            }
            temp = *env;
            while (temp)
            {
                if (ft_strcmp(temp->key, key) == 0)
                {
                    free(temp->value); 
                    temp->value = value;
                    temp->flag = (value[0] != '\0') ? 1 : 0;
                    free(key); 
                    break;
                }
                temp = temp->next;
            }
            if (!temp)
            {
                node = create_node(key, value);
                if (!node)
                {
                    free(key);
                    free(value);
                    return;
                }
                append_node(env, node);
            }

            i++;
        }
    }
}