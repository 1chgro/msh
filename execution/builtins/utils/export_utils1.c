#include "../../../minishell.h"

static t_env	*find_node_by_index(t_env *env, int target_index)
{
	t_env	*temp;

	temp = env;
	while (temp)
	{
		if (temp->index == target_index)
		{
			if (should_skip_node(temp))
				break ;
			return (temp);
		}
		temp = temp->next;
	}
	return (NULL);
}

void	print_export(t_env *env, int lst_size)
{
	t_env	*node;
	int		current_index;

	current_index = 0;
	while (current_index < lst_size)
	{
		node = find_node_by_index(env, current_index);
		if (node)
			print_node_export(node);
		current_index++;
	}
}

int	validate_export_arg(char *key, char *arg, int append)
{
	if (!key)
		return (0);
	if (!is_valid_identifier(key) || append == 2)
	{
		dup2(2, 1);
		printf("export: %s:not a valid identifier\n", arg);
		dup2(1, 2);
		free(key);
		return (0);
	}
	return (1);
}

t_env	*find_existing_node(t_env *env, char *key)
{
	t_env	*node;

	node = env;
	while (node && ft_strcmp(node->key, key))
		node = node->next;
	return (node);
}

int	handle_new_node(t_env **env, char *key, char *value,
		int have_equal)
{
	t_env	*node;

	node = create_node(key, value, have_equal);
	if (!node)
	{
		free(key);
		free(value);
		return (0);
	}
	append_node(env, node);
	return (1);
}
