/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olachgue <olachgue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:48:22 by olachgue          #+#    #+#             */
/*   Updated: 2025/06/30 19:52:19 by olachgue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_quotes(char *line, int i, char *quote, int *pos)
{
	if ((line[i] == '\'' || line[i] == '"') && *quote == 0)
	{
		*pos = i;
		*quote = line[i];
		return ;
	}
	if (line[i] == *quote && i != *pos)
		*quote = 0;
}

char	*digit_expanding(char *line, int *i, char *result)
{
	char	tmp[2];

	(*i)++;
	tmp[0] = line[*i];
	tmp[1] = '\0';
	result = ft_strjoin_ws(result, tmp);
	if (!result)
		return (NULL);
	(*i)++;
	return (result);
}

void	expand_cmd_line(t_cmd *cmd, t_glob_st *glob_strct)
{
	char	*expanded;

	if (!cmd->line)
		return ;
	expanded = NULL;
	if (check_if_export(cmd->line, glob_strct))
	{
		expanded = expand_export(cmd->line, glob_strct);
		free(cmd->line);
	}
	else
	{
		expanded = expand(cmd->line, glob_strct);
		free(cmd->line);
	}
	cmd->line = expanded;
}

void	expand_cmd_files(t_cmd *cmd, t_glob_st *glob_strct)
{
	int		i;
	char	*temp_filename;

	if (!cmd || !cmd->files)
		return ;
	(1) && (i = 0, temp_filename = NULL);
	while (cmd->files && cmd->files[i].filename)
	{
		if (cmd->files[i].type != HEREDOC)
		{
			temp_filename = cmd->files[i].filename;
			cmd->files[i].filename = expand(temp_filename, glob_strct);
			free(temp_filename);
		}
		temp_filename = cmd->files[i].filename;
		if (cmd->files[i].type == HEREDOC && check_key(temp_filename, 1))
			cmd->files[i].expand_flg = 0;
		cmd->files[i].filename = remove_outer_quotes(temp_filename);
		free(temp_filename);
		cmd->files[i].filename = restore_quotes(cmd->files[i].filename);
		if (check_for_ambgu(cmd->files[i].filename) && \
				cmd->files[i].type != HEREDOC)
			cmd->files[i].ambiguous_flg = 1;
		i++;
	}
}

void	expand_env_vars(t_glob_st *glob_strct)
{
	t_cmd	*current;

	if (!glob_strct || !glob_strct->cmd)
		return ;
	current = glob_strct->cmd;
	while (current)
	{
		expand_cmd_line(current, glob_strct);
		expand_cmd_files(current, glob_strct);
		current = current->next;
	}
}
