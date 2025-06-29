/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noel-baz <noel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:48:25 by noel-baz          #+#    #+#             */
/*   Updated: 2025/06/29 20:48:26 by noel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

char	*get_current_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (NULL);
	return (pwd);
}

int	is_valid_cwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (0);
	free(cwd);
	return (1);
}

static char	*handle_dot_path(char *stored_pwd, char *path)
{
	char	*temp_path;
	char	*result;

	if (!stored_pwd)
		return (NULL);
	temp_path = ft_strjoin_("/", path);
	if (!temp_path)
		return (NULL);
	result = ft_strjoin_(stored_pwd, temp_path);
	free(temp_path);
	return (result);
}

static char	*update_stored_path(char **stored_pwd, char *path)
{
	if (*stored_pwd)
		free(*stored_pwd);
	*stored_pwd = ft_strdup(path);
	if (!*stored_pwd)
		return (NULL);
	return (*stored_pwd);
}

char	*take_store_pwd(char *path)
{
	static char	*stored_pwd = NULL;
	char		*new_path;

	if (path)
	{
		if (!ft_strcmp(path, "."))
		{
			new_path = handle_dot_path(stored_pwd, path);
			if (!new_path)
				return (NULL);
			if (stored_pwd)
				free(stored_pwd);
			stored_pwd = new_path;
		}
		else
		{
			if (!update_stored_path(&stored_pwd, path))
				return (NULL);
		}
		return (stored_pwd);
	}
	if (stored_pwd)
		return (ft_strdup(stored_pwd));
	return (NULL);
}
