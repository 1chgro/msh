/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olachgue <olachgue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:11:05 by olachgue          #+#    #+#             */
/*   Updated: 2025/06/29 14:11:06 by olachgue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	init_cmd(t_cmd **cmd)
{
	*cmd = malloc(sizeof(t_cmd));
	if (!*cmd)
		return (0);
	(*cmd)->line = NULL;
	(*cmd)->argv = NULL;
	(*cmd)->files = NULL;
	(*cmd)->next = NULL;
	return (1);
}

int	count_redirections(t_token *tokens)
{
	int		count;
	t_token	*current;

	current = tokens;
	count = 0;
	while (current && !is_pipe(&current))
	{
		if (is_redirection(&current))
			count++;
		current = current->next;
	}
	return (count);
}

void	fill_cmd_argv(t_cmd *cmd)
{
	t_cmd	*temp_cmd;
	int		i;

	if (cmd == NULL)
		return ;
	temp_cmd = cmd;
	while (temp_cmd)
	{
		i = 0;
		temp_cmd->argv = split_line_to_args(temp_cmd->line);
		temp_cmd->argv = remove_quotes_arr(temp_cmd->argv);
		while (temp_cmd->argv && temp_cmd->argv[i])
		{
			temp_cmd->argv[i] = restore_quotes(temp_cmd->argv[i]);
			if (!temp_cmd->argv[i])
			{
				temp_cmd->argv[i] = ft_strdup("");
			}
			i++;
		}
		temp_cmd = temp_cmd->next;
	}
}

t_cmd	*create_cmd(t_glob_st *glob_strct)
{
	if (!glob_strct->tokens)
		return (NULL);
	if (!init_cmd(&glob_strct->cmd))
	{
		free_tokens(glob_strct->tokens);
		glob_strct->tokens = NULL;
		return (NULL);
	}
	if (!create_cmd_lst(glob_strct->tokens, &glob_strct->cmd))
	{
		free_cmd(glob_strct->cmd);
		glob_strct->cmd = NULL;
		return (NULL);
	}
	if (!glob_strct->cmd)
		return (NULL);
	expand_env_vars(glob_strct);
	fill_cmd_argv(glob_strct->cmd);
	return (glob_strct->cmd);
}
