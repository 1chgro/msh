#include "../../../minishell.h"

void	make_index(t_env **env)
{
	t_env	*current;
	t_env	*check;
	int		index;

	current = NULL;
	check = NULL;
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
