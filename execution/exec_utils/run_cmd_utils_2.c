/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noel-baz <noel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:50:21 by noel-baz          #+#    #+#             */
/*   Updated: 2025/06/29 20:50:22 by noel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_strjoin_(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	j = 0;
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	str = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!str)
		return (free(s1), NULL);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[j])
		str[i++] = s2[j++];
	str[i] = '\0';
	return (str);
}

t_env	*get_shlvl(t_env *env)
{
	t_env	*temp;

	temp = env;
	while (temp)
	{
		if (ft_strcmp(temp->key, "SHLVL") == 0)
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}

int	count_env_size(t_env *env)
{
	int		size_lst;
	t_env	*temp;

	size_lst = 0;
	temp = env;
	while (temp)
	{
		size_lst++;
		temp = temp->next;
	}
	return (size_lst);
}

void	fill_env_array(char **arr, t_env *env, int *i)
{
	char	*str;
	t_env	*temp;

	temp = env;
	while (temp)
	{
		str = ft_strjoin_(temp->key, "=");
		arr[(*i)++] = ft_strjoin_(str, temp->value);
		temp = temp->next;
	}
}

char	**struct_to_array(t_env *env)
{
	int		i;
	int		size_lst;
	char	**arr;

	i = 0;
	size_lst = count_env_size(env);
	arr = malloc(sizeof(char *) * (size_lst + 2));
	if (!arr)
		return (NULL);
	fill_env_array(arr, env, &i);
	if (!my_getenv("PATH", env))
	{
		arr[i] = ft_strdup("PATH=/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:");
		i++;
	}
	arr[i] = NULL;
	return (arr);
}
