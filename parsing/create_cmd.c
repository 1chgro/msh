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

static int	skip_argument(const char *str, int i)
{
	int in_quotes = 0;
	char quote_char = '\0';

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
	return (i);
}

int	count_args(char *str)
{
	int count = 0;
	int i = 0;

	if (!str)
		return (1);
	while (str[i])
	{
		if (str[i] == ' ')
		{
			i++;
			continue;
		}
		count++;
		i = skip_argument(str, i);
	}
	return (count);
}

static char *extract_arg(const char *str, int *i)
{
	int start = *i;
	int in_quotes = 0;
	char quote_char = '\0';
	char *result = NULL;

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
	result = ft_strndup(str + start, *i - start);
	return (result);
}


char *replace_value_quotes(char *value)
{
	char *new_value = NULL;
	char tmp[2];
	int i = 0;
	char quote = 0;

	if (!value)
		return (NULL);
	tmp[1] = '\0';
	while (value[i])
	{
		if (is_quote(value[i]))
		{
			if (!quote)
			{
				quote = value[i];
				tmp[0] = value[i];
			}
			else if (value[i] == quote)
			{
				quote = 0;
				tmp[0] = value[i];
			}
			else
			{
				if (value[i] == '\'')
					tmp[0] = '\x01';
				else if (value[i] == '\"')
					tmp[0] = '\x02';
			}
		}
		else
			tmp[0] = value[i];
		new_value = ft_strjoin_ws(new_value, tmp);
		i++;
	}
	free(value);
	return (new_value);
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
		return (NULL);
	j = 0;
	while (line && j < arg_count) 
	{
		while (line[i] == ' ')
			i++;
		args[j] = extract_arg(line, &i);
		args[j] = replace_value_quotes(args[j]);
		if (!args[j])
		{
			free_arr(args);
			return (NULL);
		}
		if (line[i] == ' ')
			i++;
		j++;
	}
	args[arg_count] = NULL;
	return (args);
}


void fill_cmd_argv(t_cmd *cmd)
{
	t_cmd *temp_cmd = cmd;
	int i;
	while (temp_cmd)
	{
		temp_cmd->argv = split_line_to_args(temp_cmd->line);
		if (!temp_cmd->argv && !temp_cmd->files)
		{
			temp_cmd->argv = malloc(sizeof(char *) * 2);
			temp_cmd->argv[0] = ft_strdup("");
			temp_cmd->argv[1] = NULL;
		}
		temp_cmd->argv = remove_quotes_arr(temp_cmd->argv);
		i = 0;
		while (temp_cmd->argv && temp_cmd->argv[i])
		{
			temp_cmd->argv[i] = restore_quotes(temp_cmd->argv[i]);
			if (!temp_cmd->argv[i])
			{
				free_arr(temp_cmd->argv);
				temp_cmd->argv = malloc(sizeof(char *) * 2);
				temp_cmd->argv[0] = ft_strdup("");
				temp_cmd->argv[1] = NULL;
			}
			i++;
		}
		temp_cmd = temp_cmd->next;
	}
}

static void	process_token_word(t_token *current, t_token *prev, t_cmd *temp_cmd)
{
	if (current->type == TOKEN_WORD && !is_redirection(&prev))
		temp_cmd->line = ft_strjoin(temp_cmd->line, current->value);
}

static int	process_token_redirection(t_token *current, t_cmd *temp_cmd, int *i, int count_red)
{
	if (is_redirection(&current))
	{
		if (!temp_cmd->files)
		{
			temp_cmd->files = malloc(sizeof(t_red) * (count_red + 1));
			if (!temp_cmd->files)
				return (0);
		temp_cmd->files[count_red].filename = NULL;
		temp_cmd->files[count_red].type = -1;
		temp_cmd->files[count_red].fd = -1;
		temp_cmd->files[count_red].expand_flg = 0;
		temp_cmd->files[count_red].ambiguous_flg = 0;
		}
		temp_cmd->files[*i].filename = ft_strdup(current->next->value);
		temp_cmd->files[*i].type = get_cmd_red_type(current->type);
		temp_cmd->files[*i].fd = -1;
		temp_cmd->files[*i].expand_flg = 1;
		temp_cmd->files[*i].ambiguous_flg = 0;
		(*i)++;
	}
	return (1);
}

static int handle_pipe(t_token *current, t_cmd **temp_cmd, int *count_red, int *i)
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

t_cmd *create_cmd_lst(t_token *tokens)
{
	t_cmd *cmd;
	t_cmd *temp_cmd;
	t_token *current;
	t_token *prev;
	int count_red;
	int i;

	(1) && (cmd = NULL, temp_cmd = NULL, current = tokens, prev = NULL, i = 0);
	count_red = count_redirections(tokens);
	init_cmd(&cmd);
	temp_cmd = cmd;
	while (current)
	{
		process_token_word(current, prev, temp_cmd);
		if (!process_token_redirection(current, temp_cmd, &i, count_red))
			return (free_cmd(cmd), NULL);
		if (!handle_pipe(current, &temp_cmd, &count_red, &i))
			return (free_cmd(cmd), NULL);
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
	fill_cmd_argv(glob_strct->cmd);
	return (glob_strct->cmd);
}
