/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noel-baz <noel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:37:36 by noel-baz          #+#    #+#             */
/*   Updated: 2025/06/29 14:37:38 by noel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_newline(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' && str[i + 1])
	{
		i++;
		while (str[i])
		{
			if (str[i] != 'n')
				return (1);
			i++;
		}
	}
	else
		return (1);
	return (0);
}

static void	write_newline(char **args, int i)
{
	i++;
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	printf("\n");
}

void	ft_handle_echo_arg(char **args)
{
	int	i;
	int	new_line;

	i = 0;
	new_line = check_newline(args[1]);
	if (new_line)
	{
		write_newline(args, i);
	}
	else
	{
		i = 1;
		while (args[i] && !check_newline(args[i]))
			i++;
		while (args[i])
		{
			ft_putstr_fd(args[i], 1);
			if (args[i + 1])
				ft_putstr_fd(" ", 1);
			i++;
		}
	}
}

int	ft_echo(char **s_cmd)
{
	if (!s_cmd[1])
	{
		printf("\n");
	}
	else
	{
		ft_handle_echo_arg(s_cmd);
	}
	return (0);
}
