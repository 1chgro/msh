/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noel-baz <noel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:36:29 by noel-baz          #+#    #+#             */
/*   Updated: 2025/06/29 14:36:32 by noel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	all(char *path)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (path[i])
	{
		if (path[i] == '.' && path[i + 1] == '.' && path[i + 2] != '.')
		{
			len++;
			i = i + 2;
		}
		else
			return (-1);
		if (path[i] && path[i] == '/')
			i++;
	}
	return (len);
}

char	*get_old_pwd_or_default(void)
{
	char	*old_pwd;
	char	*stored_pwd;

	old_pwd = get_current_pwd();
	if (!old_pwd)
	{
		stored_pwd = take_store_pwd(NULL);
		if (!stored_pwd)
		{
			old_pwd = ft_strdup("/");
			if (!old_pwd)
				ft_putstr_fd("msh: cd: cannot allocate memory\n", 2);
		}
		else
			old_pwd = stored_pwd;
	}
	return (old_pwd);
}

char	*handle_no_args(t_env *env, char *old_pwd)
{
	char	*path;

	path = get_home(env);
	if (!path)
	{
		ft_putstr_fd("msh: cd: HOME not set\n", 2);
		free(old_pwd);
		return (NULL);
	}
	if (path[0] == '\0')
		path = ft_strdup(".");
	return (path);
}

char	*handle_tilde_only(t_env *env, char *old_pwd)
{
	char	*path;

	path = get_home(env);
	if (!path)
	{
		ft_putstr_fd("msh: cd: HOME not set\n", 2);
		free(old_pwd);
		return (NULL);
	}
	if (path[0] == '\0')
		path = ft_strdup(".");
	return (path);
}

char	*handle_dash_only(t_env *env, char *old_pwd)
{
	char	*path;

	path = my_getenv("OLDPWD", env);
	if (!path)
	{
		ft_putstr_fd("msh: cd: OLDPWD not set\n", 2);
		free(old_pwd);
		return (NULL);
	}
	else if (path && path[0] != '\0')
		printf("%s\n", path);
	return (path);
}
