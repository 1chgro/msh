#include "../../../minishell.h"

int	check_append_op(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '+' && str[i] != '=')
		i++;
	if (i > 0 && str[i] == '+' && str[i + 1] != '=')
		return (2);
	if (i > 0 && str[i] == '+' && str[i + 1] == '=')
		return (1);
	return (0);
}

void	update_node_value(t_env *node, char *value, int append, int have_equal)
{
	char	*new_value;

	if (append && node->flag)
	{
		new_value = ft_strjoin_(node->value, value);
		free(node->value);
		free(value);
		node->value = new_value;
		if (have_equal)
			node->flag = 1;
		else
			node->flag = 0;
	}
	else
	{
		if (node->value && !value)
			return ;
		free(node->value);
		node->value = value;
		if (have_equal)
			node->flag = 1;
		else
			node->flag = 0;
	}
}

int	is_valid_identifier(char *key)
{
	int	i;

	if (!key || !key[0] || ft_isdigit(key[0]))
		return (0);
	i = 0;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	should_skip_node(t_env *temp)
{
	if ((!ft_strcmp(temp->key, "PATH") && temp->flag == 2)
		|| !ft_strcmp(temp->key, "_") || temp->flag == 2)
		return (1);
	return (0);
}

void	print_node_export(t_env *temp)
{
	ft_putstr_fd("declare -x ", 1);
	ft_putstr_fd(temp->key, 1);
	if (temp->flag)
	{
		ft_putstr_fd("=\"", 1);
		ft_putstr_fd(temp->value, 1);
		ft_putstr_fd("\"", 1);
	}
	ft_putendl_fd("", 1);
}
