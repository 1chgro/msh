/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_cmd_lst.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olachgue <olachgue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:47:40 by olachgue          #+#    #+#             */
/*   Updated: 2025/06/29 20:47:59 by olachgue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	process_token_word(t_token *current, t_token *prev, t_cmd *temp_cmd)
{
	if (current->type == TOKEN_WORD && !is_redirection(&prev))
	{
		temp_cmd->line = ft_strjoin(temp_cmd->line, current->value);
	}
}

static int	init_cmd_files(t_cmd *temp_cmd, int count_red)
{
	int	j;

	temp_cmd->files = malloc(sizeof(t_red) * (count_red + 1));
	if (!temp_cmd->files)
		return (0);
	j = 0;
	while (j <= count_red)
	{
		temp_cmd->files[j].filename = NULL;
		temp_cmd->files[j].type = -1;
		temp_cmd->files[j].fd = -1;
		temp_cmd->files[j].expand_flg = 0;
		temp_cmd->files[j].ambiguous_flg = 0;
		j++;
	}
	return (1);
}

static int	process_token_redirection(t_token *current, \
		t_cmd *temp_cmd, int *i, int count_red)
{
	if (is_redirection(&current))
	{
		if (!temp_cmd->files)
		{
			if (!init_cmd_files(temp_cmd, count_red))
				return (0);
		}
		temp_cmd->files[*i].filename = ft_strdup(current->next->value);
		if (!temp_cmd->files[*i].filename)
			return (0);
		temp_cmd->files[*i].type = get_cmd_red_type(current->type);
		temp_cmd->files[*i].fd = -1;
		temp_cmd->files[*i].expand_flg = 1;
		temp_cmd->files[*i].ambiguous_flg = 0;
		(*i)++;
	}
	return (1);
}

static int	handle_pipe(t_token *current, \
			t_cmd **temp_cmd, int *count_red, int *i)
{
	if (is_pipe(&current))
	{
		if (!init_cmd(&(*temp_cmd)->next))
			return (0);
		*temp_cmd = (*temp_cmd)->next;
		*count_red = count_redirections(current->next);
		*i = 0;
	}
	return (1);
}

int	create_cmd_lst(t_token *tokens, t_cmd **cmd)
{
	t_cmd	*temp_cmd;
	t_token	*current;
	t_token	*prev;
	int		count_red;
	int		i;

	(1) && (temp_cmd = NULL, current = tokens, prev = NULL, i = 0);
	count_red = count_redirections(tokens);
	temp_cmd = *cmd;
	while (current)
	{
		process_token_word(current, prev, temp_cmd);
		if (!process_token_redirection(current, temp_cmd, &i, count_red))
			return (free_tokens(tokens), 0);
		if (!handle_pipe(current, &temp_cmd, &count_red, &i))
			return (free_tokens(tokens), 0);
		prev = current;
		current = current->next;
	}
	free_tokens(tokens);
	return (1);
}
