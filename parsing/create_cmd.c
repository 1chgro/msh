#include "../minishell.h"

void init_cmd(t_cmd **cmd)
{
	*cmd = malloc(sizeof(t_cmd));
	if (!*cmd)
		return;
	(*cmd)->line = NULL;
	(*cmd)->argv = NULL;
	(*cmd)->files = NULL;
	(*cmd)->next = NULL;
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



static int count_args(const char *str) {
	int count = 0;
	int i = 0;
	int in_quotes = 0;
	char quote_char = '\0';

	while (str[i])
	{
		if (str[i] == ' ') {
			i++;
			continue;
		}
		count++;
		while (str[i]) {
			if (is_quote(str[i])) {
				if (!in_quotes)
				{
					quote_char = str[i];
					in_quotes = 1;
				}
				else if (str[i] == quote_char)
					in_quotes = 0;
			} else if (str[i] == ' ' && !in_quotes) {
				break;
			}
			i++;
		}
	}
	return count;
}

static char *extract_arg(const char *str, int *i) {
	int start = *i;
	int in_quotes = 0;
	char quote_char = '\0';

	while (str[*i]) {
		if (is_quote(str[*i])) {
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

char **split_line_to_args(char *line) {
	int i = 0;

	if (!line)
		return (NULL);
	int arg_count = count_args(line);
	char **args = malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
		return NULL;
	int j = 0;
	while ( j < arg_count) 
	{
		while (line[i] == ' ')
			i++;
		args[j] = extract_arg(line, &i);
		if (!args[j])
		{
			while (--j >= 0)
				free(args[j]);
			free(args);
			return NULL;
		}
		if (line[i] == ' ')
			i++;
		j++;
	}
	args[arg_count] = NULL;
	return args;
}


void fill_cmd_argv(t_cmd *cmd)
{
	t_cmd *temp_cmd = cmd;

	while (temp_cmd)
	{
		// temp_cmd->argv = ft_split(temp_cmd->line, ' ');
		temp_cmd->argv = split_line_to_args(temp_cmd->line);
		if (!temp_cmd->argv)
			return ;
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

	// knahsb awal blocj ch7al fih men redirection o kan initializi cmd first node.
	count_red = count_redirections(tokens);
	init_cmd(&cmd);
	temp_cmd = cmd;
	while (current)
	{
		// hna kangad string men l cmd o l args dyalo bach n splitihom f lkhr
		// kola block kan3awd ndirha fih
		if (current->type == TOKEN_WORD && !is_redirection(&prev))
			temp_cmd->line = ft_strjoin(temp_cmd->line, current->value);
		// hna redirections knakhd type dyalhommo kanstory l filename li ja men bbed redirection
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
			i++;
		}
		// ila kant pipe go to next cmd o initializih o 3awd l count dyal redirections l fdak l block
		if (is_pipe(&current))
		{
			init_cmd(&temp_cmd->next);
			temp_cmd = temp_cmd->next;
			count_red = count_redirections(current->next);
			i = 0;
		}
		prev = current;
		current = current->next;
	}
	return (cmd);
}

t_cmd *create_cmd(t_token *tokens, t_env *env)
{
	t_cmd *cmd;

	cmd = NULL;
	if (!tokens)
		return (NULL);
	cmd = create_cmd_lst(tokens);
	if (!cmd)
		return (NULL);
	expand_env_vars(cmd, env);
	fill_cmd_argv(cmd);
	return (cmd);
}
