#include "../minishell.h"

void free_env(t_env *env)
{
	t_env *tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

void free_split(char **split)
{
    int i = 0;
    if (!split)
        return;
    while (split[i])
    {
        free(split[i]);
        i++;
    }
    free(split);
}

char    *check_dolar(char *str)
{
    int i;
    char *key;

    i = 0;

    if (ft_strchr(str, '$'))
    {
        key = ft_strdup("");
        while (str[i] && str[i] != '$')
        {
            key[i] = str[i];
            i++;
        }
        free(str);
        return (key);
    }
    return(str);
}
char *get_key(char *str)
{
    char *key;
    int i;
    int j;

    i = 0;
    while (str[i] && str[i] != '=' && str[i] != '+')
        i++;
    key = malloc(i + 1);
    if (!key)
        return (NULL);
    j = 0;
    while (j < i)
    {
        key[j] = str[j];
        j++;
    }
    key[j] = '\0';

    return (check_dolar(key));
}

char *get_value(char *str)
{
    char *value;
    int i;
    int len;
    int j;

    i = 0;
    while (str[i] && str[i] != '=')
        i++;
    if (str[i] == '=')
        i++;
    len = 0;
    while (str[i + len])
        len++;
    value = malloc(len + 1);
    if (!value)
        return (NULL);
    j = 0;
    while (str[i])
    {
        value[j] = str[i];
        i++;
        j++;
    }
    value[j] = '\0';
    return (value);
}

t_env *create_node(char *key, char *value)
{
    t_env *node = malloc(sizeof(t_env));
    if (!node)
        return (NULL);
    node->key = key;
    if (!value || value[0] == '\0')
        node->value = ft_strdup("");
    else
        node->value = value;
    node->flag = (value && value[0] != '\0') ? 1 : 0;
    node->index = -1;
    node->next = NULL;
    return (node);
}

void append_node(t_env **head, t_env *node)
{
    t_env *tail;

    if (!*head)
    {
        *head = node;
        return;
    }
    tail = *head;
    while (tail->next)
        tail = tail->next;
    tail->next = node;
}
char    **fill_env()
{
    char    **env;

    env = malloc(sizeof(char *) * 6);
    if (!env)
        return (NULL);
    env[0] = ft_strdup("OLDPWD");
    env[1] = ft_strjoin_("PWD=", get_current_pwd());
    env[2] = ft_strdup("SHLVL=1");
    env[3] = ft_strdup("_=/usr/bin/env");
    env[4] = ft_strdup("PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
    env[5] = NULL;
    return(env);
}
int copie_env(t_env **c_env, char **env)
{
    t_env *node;
    char *key;
    char *value;
    int i = 0;

    *c_env = NULL;
    if (!*env)
    {
        env = fill_env();
        while (env[i])
        {
            key = get_key(env[i]);
            if (!key)
                return (0);
            value = get_value(env[i]);
            if (!value)
            {
                free(key);
                return (0);
            }
            node = create_node(key, value);
            if (!node)
            {
                free(key);
                free(value);
                return (0);
            }
            if (ft_strcmp(node->key, "PATH") == 0)
                node->flag = 2;
            append_node(c_env, node);
            i++;
        }
        return (1);
    }
    while (env[i])
    {
        key = get_key(env[i]);
        if (!key)
            return (0);
        value = get_value(env[i]);
        if (!value)
        {
            free(key);
            return (0);
        }
        node = create_node(key, value);
        if (!node)
        {
            free(key);
            free(value);
            return (0);
        }
        append_node(c_env, node);
        i++;
    }
    return (1);
}