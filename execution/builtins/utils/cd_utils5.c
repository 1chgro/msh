/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noel-baz <noel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:48:36 by noel-baz          #+#    #+#             */
/*   Updated: 2025/06/29 20:48:37 by noel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static char	*navigate_up_directories(char *old_pwd, int nbr)
{
	char	*temp_path;
	char	*to_rm;

	temp_path = ft_strdup(old_pwd);
	if (!temp_path)
		return (NULL);
	while (nbr > 0)
	{
		to_rm = ft_strrchr(temp_path, '/');
		temp_path = process_up_navigation(temp_path, to_rm);
		if (!temp_path)
			return (NULL);
		if (ft_strcmp(temp_path, "/") == 0)
			break ;
		nbr--;
	}
	return (temp_path);
}

static int	handle_dotdot_chdir_fail(char *path, char *old_pwd)
{
	int		nbr;
	char	*temp_path;

	nbr = all(path);
	if (ft_strcmp(path, "..") != 0 && nbr <= 0)
		return (1);
	if (nbr < 0)
		nbr = 1;
	temp_path = navigate_up_directories(old_pwd, nbr);
	if (!temp_path)
	{
		ft_putstr_fd("msh: cd: cannot allocate memory\n", 2);
		return (1);
	}
	if (chdir(temp_path) == -1)
	{
		free(temp_path);
		perror("msh: cd");
		return (1);
	}
	free(temp_path);
	return (0);
}

int	attempt_chdir(char *path, char *old_pwd)
{
	if (chdir(path) == -1)
	{
		if (handle_dotdot_chdir_fail(path, old_pwd) != 0)
		{
			perror("msh: cd");
			return (1);
		}
	}
	return (0);
}
