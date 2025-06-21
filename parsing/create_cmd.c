#include "../minishell.h"

int init_cmd(t_cmd **cmd)
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

int count_redirections(t_token *tokens)
{
	int count = 0;
	t_token *current = tokens;

	while (current && !is_pipe(&current))
	{
		if (is_redirection(&current))
			count++;
		current = current->next;
	}
	return (count);
}

t_redirection_type get_cmd_red_type(t_token_type type)
{
	if (type == TOKEN_REDIRECT_IN)
		return (REDIRECT_IN);
	else if (type == TOKEN_REDIRECT_OUT)
		return (REDIRECT_OUT);
	else if (type == TOKEN_APPEND)
		return (APPEND);
	else if (type == TOKEN_HEREDOC)
		return (HEREDOC);
	return (-1);
}


static int count_args(const char *str)
{
	int count = 0;
	int i = 0;
	int in_quotes = 0;
	char quote_char = '\0';

	while (str[i])
	{
		if (str[i] == ' ')
		{
			i++;
			continue;
		}
		count++;
		while (str[i])
		{
			if (is_quote(str[i]))
			{
				if (!in_quotes)
				{
					quote_char = str[i];
					in_quotes = 1;
				}
				else if (str[i] == quote_char)
					in_quotes = 0;
			}
			else if (str[i] == ' ' && !in_quotes)
				break;
			i++;
		}
	}
	return (count);
}

static char *extract_arg(const char *str, int *i)
{
	int start = *i;
	int in_quotes = 0;
	char quote_char = '\0';

	while (str[*i])
	{
		if (is_quote(str[*i]))
		{
			if (!in_quotes)
			{
				quote_char = str[*i];
				in_quotes = 1;
			}
			else if (str[*i] == quote_char)
				in_quotes = 0;
		} 
		else if (str[*i] == ' ' && !in_quotes) 
			break;
		(*i)++;
	}
	return (ft_strndup(str + start, *i - start));
}

char **split_line_to_args(char *line)
{
	int i;
	int arg_count;
	char **args;
	int j;

	if (!line)
		return (NULL);
	i = 0;
	arg_count = count_args(line);
	args = malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
		return (free(line), NULL);
	j = 0;
	while (j < arg_count) 
	{
		while (line[i] == ' ')
			i++;
		args[j] = extract_arg(line, &i);
		if (!args[j])
			return (free_arr(args), NULL);
		if (line[i] == ' ')
			i++;
		j++;
	}
	return (args[arg_count] = NULL, args);
}


void fill_cmd_argv(t_cmd *cmd, t_glob_st *glob_strct)
{
	t_cmd *temp_cmd = cmd;

	while (temp_cmd)
	{
		temp_cmd->argv = split_line_to_args(temp_cmd->line);
		temp_cmd->argv = remove_quotes_arr(temp_cmd->argv);
		temp_cmd = temp_cmd->next;
	}
}

t_cmd *create_cmd_lst(t_token *tokens)
{
	t_cmd *cmd;
	cmd = NULL;
	t_cmd *temp_cmd = NULL;
	t_token *current = tokens;
	t_token *prev = NULL;
	int count_red = 0;
	int i = 0;
	count_red = count_redirections(tokens);
	init_cmd(&cmd);
	temp_cmd = cmd;
	while (current)
	{
		if (current->type == TOKEN_WORD && !is_redirection(&prev))
			temp_cmd->line = ft_strjoin(temp_cmd->line, current->value);
		if (is_redirection(&current))
		{
			if (temp_cmd->files == NULL)
				temp_cmd->files = malloc(sizeof(t_red) * (count_red + 1));
			if (!temp_cmd->files)
				return (NULL);
			temp_cmd->files[count_red] = (t_red){NULL, -1, -1};
			temp_cmd->files[i].filename = ft_strdup(current->next->value);
			temp_cmd->files[i].type = get_cmd_red_type(current->type);
			temp_cmd->files[i].fd = -1;
			temp_cmd->files[i].expand_flg = 1;
			i++;
		}
		if (is_pipe(&current))
		{
			if (!init_cmd(&temp_cmd->next))
				return (free_cmd(cmd), NULL);
			temp_cmd = temp_cmd->next;
			count_red = count_redirections(current->next);
			i = 0;
		}
		prev = current;
		current = current->next;
	}
	return (cmd);
}

t_cmd *create_cmd(t_glob_st *glob_strct)
{
	if (!glob_strct->tokens)
		return (NULL);
	glob_strct->cmd = create_cmd_lst(glob_strct->tokens);
	free_tokens(glob_strct->tokens);
	if (!glob_strct->cmd)
		return (NULL);
	expand_env_vars(glob_strct);
	fill_cmd_argv(glob_strct->cmd, glob_strct);
	return (glob_strct->cmd);
}
