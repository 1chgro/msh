#include "../minishell.h"

char *my_getenv2(char *name, t_env *env)
{
	t_env *temp = env;
	
	while (temp)
	{
		if (ft_strcmp(temp->key, name) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return (NULL);
}

char *ft_strndup(const char *s, int n)
{
	char *dup;
	int i;
	
	if (!s || n < 0)
		return (NULL);
	
	dup = malloc(sizeof(char) * (n + 1));
	if (!dup)
		return (NULL);
	
	i = 0;
	while (s[i] && i < n)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	
	return (dup);
}

int is_valid_char(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || \
		(c >= '0' && c <= '9')|| (c == '_');
}

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

void handle_quotes(char *line, int i, char *quote, int *pos)
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

char *handle_digit_expansion(char *line, int *i, char *result)
{
	char tmp[2];
	(*i)++;
	tmp[0] = line[*i];
	tmp[1] = '\0';
	result = ft_strjoin_ws(result, tmp);
	(*i)++;
	return (result);
}

char *replace_quotes(char *value)
{
	char *new_value;
	char tmp[2];
	if (!value)
		return (NULL);
	tmp[0] = '\0';
	tmp[1] = '\0';
	int i = 0;
	new_value = NULL;
	while (value[i])
	{
		if (value[i] == '\'')
			tmp[0] = '\x01';
		else if (value[i] == '\"')
			tmp[0] = '\x02';
		else
			tmp[0] = value[i];
		tmp[1] = '\0';
		new_value = ft_strjoin_ws(new_value, tmp);
		i++;
	}
	return (new_value);
}

char *restore_quotes(char *value)
{
	char *new_value;
	char tmp[2];
	if (!value)
		return (NULL);

	new_value = NULL;
	tmp[0] = '\0';
	tmp[1] = '\0';
	int i = 0;
	while (value[i])
	{
		if (value[i] == '\x01')
			tmp[0] = '\'';
		else if (value[i] == '\x02')
			tmp[0] = '\"';
		else
			tmp[0] = value[i];
		tmp[1] = '\0';
		new_value = ft_strjoin_ws(new_value, tmp);
		i++;
	}
	return (new_value);
}

char *handle_variable_expansion(char *line, int *i, t_glob_st *glob_strct, char *result)
{
	char *var_value;
	char *var;
	int j;
	
	(1) && (var_value = NULL, var = NULL, j = 0);
	(*i)++;
	if (ft_isdigit(line[*i]) && is_valid_char(line[*i + 1]))
		return handle_digit_expansion(line, i, result);
	if (line[*i] == '?')
	{
		result = ft_strjoin_ws(result, ft_itoa(glob_strct->ext_stat));
		return ((*i)++, result);
	}
	j = *i;
	while (line[j] && is_valid_char(line[j]))
		j++;
	var = ft_strndup(&line[*i], j - *i);
	var_value = my_getenv2(var, glob_strct->env);
	var_value = replace_quotes(var_value);
	if (var_value)
		result = ft_strjoin_ws(result, var_value);
	else
		result = ft_strjoin_ws(result, "");
	*i = j;
	return (free(var), result);
}

static int handle_dollar_case(char *line, int *i, char quote)
{
	if (line[*i] == '$' && line[*i + 1] == '$')
	{
		*i += 2;
		return (1);
	}
	else if (line[*i] == '$' && !is_valid_char(line[*i + 1]) && quote == 0)
	{
		(*i)++;
		return (1);
	}
	return (0);
}

char *expand_core(char *line, t_glob_st *glob_strct)
{
	int i;
	int pos;
	char quote;
	char tmp[2];
	char *result = NULL;
	(1) && (i = 0, quote = 0, pos = 0, tmp[0] = '\0', tmp[1] = '\0');
	while (line[i])
	{
		handle_quotes(line, i, &quote, &pos);
		if (line[i] == '$' && (is_valid_char(line[i + 1]) || line[i + 1] == '?') && quote != '\'')
			result = handle_variable_expansion(line, &i, glob_strct, result);
		else
		{
			if (handle_dollar_case(line, &i, quote))
				continue;
			if (line[i])
			tmp[0] = line[i++];
			tmp[1] = '\0';
			result = ft_strjoin_ws(result, tmp);
		}
	}
	return (result);
}


char *expand(char *line, t_glob_st *glob_strct)
{
	char *result;

	if (!line)
		return (NULL);
	result = expand_core(line, glob_strct);
	if (result == NULL)
		return (ft_strdup(""));
	return (result);
}


char *ft_strcpy(char *dest, const char *src)
{
	char *ptr = dest;
	while (*src)
		*ptr++ = *src++;
	*ptr = '\0';
	return dest;
}

char *space_change(char *str)
{
	char *new_str;
	if (!str)
		return (NULL);
	char tmp[2];
	tmp[0] = '\0';
	tmp[1] = '\0';
	int i = 0;
	new_str = NULL;
	while(str[i])
	{
		if (is_space(str[i]))
			tmp[0] = ' ';
		else
			tmp[0] = str[i];
		tmp[1] = '\0';
		new_str = ft_strjoin_ws(new_str, tmp);
		i++;
	}
	return (new_str);
}

static void expand_cmd_line(t_cmd *cmd, t_glob_st *glob_strct)
{
	char *expanded;

	expanded = NULL;
	if (!cmd->line)
		return;
	if (check_if_export(cmd->line, glob_strct))
	{
		expanded = expand_export(cmd->line, glob_strct);
	}
	else
	{
		expanded = expand(cmd->line, glob_strct);
		expanded = space_change(expanded);
	}
	
	cmd->line = expanded;
}

int check_for_ambgu(char *name)
{
	int i;

	if (!name || !*name)
		return (1);
	while(name[i])
	{
		if (is_space(name[i]))
			return (1);
		i++;
	}
	return (0);
}

static void expand_cmd_files(t_cmd *cmd, t_glob_st *glob_strct)
{
	int i;

	i = 0;
	while (cmd->files && cmd->files[i].filename)
	{
		if (cmd->files[i].type != HEREDOC)
			cmd->files[i].filename = expand(cmd->files[i].filename, glob_strct);
		if (cmd->files[i].type == HEREDOC && check_key(cmd->files[i].filename))
			cmd->files[i].expand_flg = 0;
		cmd->files[i].filename = remove_outer_quotes(cmd->files[i].filename);
		if (check_for_ambgu(cmd->files[i].filename))
			cmd->files[i].ambiguous_flg = 1;
		i++;
	}
}

void expand_env_vars(t_glob_st *glob_strct)
{
	t_cmd *current = glob_strct->cmd;

	while (current)
	{
		expand_cmd_line(current, glob_strct);
		expand_cmd_files(current, glob_strct);
		current = current->next;
	}
}
