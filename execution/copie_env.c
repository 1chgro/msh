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

char *get_key(char *str)
{
    char *key;
    int i = 0;

    while (str[i] && str[i] != '=')
        i++;
    key = malloc(i + 1);
    if (!key)
        return (NULL);
    for (int j = 0; j < i; j++)
        key[j] = str[j];
    key[i] = '\0';
    return (key);
}

char *get_value(char *str)
{
    char *value;
    int i = 0;
    int len = 0;

    while (str[i] && str[i] != '=')
        i++;
    if (str[i] == '=')
        i++;
    len = strlen(str + i);
    value = malloc(len + 1);
    if (!value)
        return (NULL);
    for (int j = 0; j < len; j++)
        value[j] = str[i + j];
    value[len] = '\0';
    return (value);
}

t_env *create_node(char *key, char *value)
{
    t_env *node = malloc(sizeof(t_env));
    if (!node)
        return (NULL);
    node->key = key;
    if (!value || value[0] == '\0')
        node->value = strdup(" ");
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

int copie_env(t_env **c_env, char **env)
{
    t_env *node;
    char *key;
    char *value;
    int i = 0;

    *c_env = NULL;
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