#include "../parse.h"

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

static t_env *create_node(char **split_env)
{
	t_env *node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = split_env[0];
	node->value = split_env[1];
	node->next = NULL;
	free(split_env);
	return (node);
}

static void append_node(t_env **head, t_env *node)
{
	t_env *tail;

	if (!*head)
	{
		*head = node;
		return ;
	}
	tail = *head;
	while (tail->next)
		tail = tail->next;
	tail->next = node;
}

int copie_env(t_env **c_env, char **env)
{
	t_env *node;
	char **split_env;
	int i;

	i = 0;
	while (env[i])
	{
		split_env = ft_split(env[i], '=');
		if (!split_env)
			return (0);
		node = create_node(split_env);
		if (!node)
			return (0);
		append_node(c_env, node);
		i++;
	}
	return (1);
}