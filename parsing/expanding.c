#include "../minishell.h"

// int is_export(char *line)
// {
//     if (!line)
//         return (0);
//     if (ft_strncmp(line, "export", 6) == 0)
//         return (1);
//     return (0);
// }

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
    if (var_value)
        result = ft_strjoin_ws(result, var_value);
    else
        result = ft_strjoin_ws(result, "");
    *i = j;
    return (free(var), result);
}

char *expand_core(char *line, t_glob_st *glob_strct, char *result)
{
    int i;
    int pos;
    char quote;
    char tmp[2];

    (1) && (i = 0, quote = 0, pos = 0, tmp[0] = '\0', tmp[1] = '\0');
    while (line[i])
    {
        handle_quotes(line, i, &quote, &pos);
        if (line[i] == '$' && (is_valid_char(line[i + 1]) || line[i + 1] == '?') && quote != '\'')
            result = handle_variable_expansion(line, &i, glob_strct, result);
        else
        {
            if (line[i] == '$' && !is_valid_char(line[i + 1]) && quote == 0)
            {
                i++;
                continue;
            }
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
    result = malloc(1);
    result[0] = '\0';
    return (expand_core(line, glob_strct, result));
}


char *ft_strcpy(char *dest, const char *src)
{
    char *ptr = dest;
    while (*src)
        *ptr++ = *src++;
    *ptr = '\0';
    return dest;
}

void expand_env_vars(t_glob_st *glob_strct)
{
    t_cmd *current = glob_strct->cmd;
    char *expanded = NULL;
    int i ;
    while (current)
    {
        i = 0;
        if (current->line && is_export(current->line))
        {
            printf("is_export: %s\n", current->line);
            expanded = expand_export(current->line, glob_strct);

        }
        else if (current->line)
        {
            expanded = expand(current->line, glob_strct);
        }
        current->line = expanded;
        while (current->files && current->files[i].filename)
        {
            if (current->files[i].type != HEREDOC)
                current->files[i].filename = expand(current->files[i].filename, glob_strct);
            current->files[i].filename = remove_outer_quotes(current->files[i].filename);
            i++;
        }
        current = current->next;
    }
}
