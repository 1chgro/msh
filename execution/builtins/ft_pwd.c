/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noel-baz <noel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:49:15 by noel-baz          #+#    #+#             */
/*   Updated: 2025/06/29 20:59:10 by noel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_pwd(t_env *env)
{
	char	*pwd;

	(void)env;
	if (!is_valid_cwd())
	{
		pwd = take_store_pwd(NULL);
		if (!pwd)
		{
			ft_putstr_fd("pwd: error retrieving current directory: getcwd: "
				"cannot access parent directories:"
				" No such file or directory\n", 2);
			return (1);
		}
		printf("%s\n", pwd);
		free(pwd);
		return (0);
	}
	pwd = getcwd(NULL, 0);
	take_store_pwd(pwd);
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}
