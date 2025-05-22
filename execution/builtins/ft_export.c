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

// void    ft_export(char  **s_cmd, t_env **env)
// {
//     int current_index;
//     int lst_size;
//     t_env   *temp = NULL;
//     t_env   *node;
//     char    *key;
//     char    *value;
//     int     i;

//     current_index = 0;
//     lst_size = 0;
//     i = 0;
//     make_index(env);
//     temp = *env;
//     if (!s_cmd[1])
//     {
//         while (temp)
//         {
//             lst_size++;
//             temp = temp->next;
//         }
//         while (current_index < lst_size)
//         {
//             temp = *env;
//             while (temp)
//             {
//                 if (temp->index == current_index)
//                 {
//                     if (temp->flag)
//                         printf("declare -x %s=\"%s\"  index = %d\n", temp->key, temp->value, temp->index);
//                     else
//                         printf("declare -x %s  index = %d\n", temp->key, temp->index);
//                     break;
//                 }
//                 temp = temp->next;
//             }
//             current_index++;
//         }
//     }
//     else
//     {
//         i = 1;
//         while (s_cmd[i])
//         {
//             if (!check_valid_identifier(s_cmd[i]))
                
//             key = get_key(s_cmd[i]);
//             if (!key)
//             {
//                 return;
//             }
//             value = get_value(s_cmd[i]);
//             if (!value)
//             {
//                 free(key);
//                 return;
//             }
//             temp = *env;
//             while (temp)
//             {
//                 if (ft_strcmp(temp->key, key) == 0)
//                 {
//                     free(temp->value); 
//                     temp->value = value;
//                     temp->flag = (value) ? 1 : 0;
//                     free(key); 
//                     break;
//                 }
//                 temp = temp->next;
//             }
//             if (!temp)
//             {
//                 node = create_node(key, value);
//                 if (!node)
//                 {
//                     free(key);
//                     free(value);
//                     return;
//                 }
//                 append_node(env, node);
//             }
//             i++;
//         }
//     }
// }
/* ft_export.c */
// void	ft_putstr_fd(char *s, int fd)
// {
// 	int	i;

// 	if (fd < 0 || !s)
// 		return ;
// 	i = 0;
// 	while (s[i])
// 	{
// 		write(fd, &s[i], 1);
// 		i++;
// 	}
// }
void	ft_putchar_fd(char c, int fd)
{
	if (fd < 0)
		return ;
	write(fd, &c, 1);
}
void	ft_putendl_fd(char *s, int fd)
{
	int	i;

	if (fd < 0 || !s)
		return ;
	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
	ft_putchar_fd('\n', fd);
}
int	ft_isalnum(int c)
{
	return (((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		|| (c >= '0' && c <= '9'));
}
int check_append_op(char *str)
{
    int i;

    i = 0;
    while (str[i] && str[i] != '=')
        i++;
    if (i > 0 && str[i - 1] == '+')
        return (1);
    return (0);
}
void update_node_value(t_env *node, char *value, int append)
{
    char *new_value;

    if (append && node->flag)
    {
        new_value = ft_strjoin_(node->value, value);
        free(node->value);
        free(value);
        node->value = new_value;
        node->flag = (new_value && new_value[0] != '\0') ? 1 : 0;
    }
    else
    {
        free(node->value);
        node->value = value;
        node->flag = (value && value[0] != '\0') ? 1 : 0;
    }
}
static int is_valid_identifier(char *key)
{
    int i;

    if (!key || !key[0] || ft_isdigit(key[0]))
        return (0);
    i = 0;
    while (key[i])
    {
        if (!ft_isalnum(key[i]) && key[i] != '_' && key[i] != '$')
            return (0);
        i++;
    }
    return (1);
}

static void print_export(t_env *env, int lst_size)
{
    t_env *temp;
    int current_index;

    current_index = 0;
    while (current_index < lst_size)
    {
        temp = env;
        while (temp)
        {
            if (temp->index == current_index)
            {
                if((!ft_strcmp(temp->key, "PATH") && temp->flag == 2) || !ft_strcmp(temp->key, "_"))
                    break;
                ft_putstr_fd("declare -x ", 1);
                ft_putstr_fd(temp->key, 1);
                if (temp->flag)
                {
                    ft_putstr_fd("=\"", 1);
                    ft_putstr_fd(temp->value, 1);
                    ft_putstr_fd("\"", 1);
                }
                ft_putendl_fd("", 1);
                break;
            }
            temp = temp->next;
        }
        current_index++;
    }
}

static int process_export_arg(char *arg, t_env **env)
{
    char *key;
    char *value;
    int append;
    t_env *node;

    
    append = check_append_op(arg);
    key = get_key(arg);
    if (!is_valid_identifier(key))
    {
        ft_putstr_fd("export: `", 2);
        ft_putstr_fd(arg, 2);
        ft_putendl_fd("': not a valid identifier", 2);
        free(key);
        return (0);
    }
    value = get_value(arg);
    if (!key || !value)
    {
        free(key);
        free(value);
        return (0);
    }
    node = *env;
    while (node && ft_strcmp(node->key, key))
        node = node->next;
    if (node)
    {
        update_node_value(node, value, append);
        free(key);
    }
    else
    {
        node = create_node(key, value);
        if (!node)
        {
            free(key);
            free(value);
            return (0);
        }
        append_node(env, node);
    }
    return (1);
}

void ft_export(char **s_cmd, t_env **env)
{
    t_env *temp;
    int lst_size;
    int i;
    // int status;

    make_index(env);
    temp = *env;
    lst_size = 0;
    // status = 0;
    if (!s_cmd[1])
    {
        while (temp)
        {
            lst_size++;
            temp = temp->next;
        }
        print_export(*env, lst_size);
        // return (0);
        return ;
    }
    i = 1;
    while (s_cmd[i])
    {
        // if (!process_export_arg(s_cmd[i], env))
        // if ()
        process_export_arg(s_cmd[i], env);
        i++;
        // status = 1;
    }
    // return (status);
}