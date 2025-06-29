/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olachgue <olachgue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:10:44 by olachgue          #+#    #+#             */
/*   Updated: 2025/06/29 14:10:45 by olachgue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		tmp->value = NULL;
		free(tmp);
		tmp = NULL;
	}
}

void	free_arr(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	arr = NULL;
}

void	free_cmd_files(t_red *files)
{
	int	i;

	if (!files)
		return ;
	i = 0;
	while (files[i].filename)
	{
		free(files[i].filename);
		i++;
	}
	free(files);
	files = NULL;
}

void	free_cmd(t_cmd *cmd)
{
	t_cmd	*temp;

	if (!cmd)
		return ;
	while (cmd)
	{
		free(cmd->line);
		free_arr(cmd->argv);
		free_cmd_files(cmd->files);
		temp = cmd;
		cmd = cmd->next;
		free(temp);
		temp = NULL;
	}
}
