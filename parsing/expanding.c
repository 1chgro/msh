#include "../minishell.h"

int is_export(char *line)
{
    if (!line)
        return (0);
    if (ft_strncmp(line, "export", 6) == 0)
        return (1);
    return (0);
}

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


char *expand(char *line, t_glob_st *glob_strct)
{
    char *result = malloc(1);
    char *var_value = NULL;
    int i = 0, j;
    char quote = 0;
    result[0] = '\0';
    char tmp[2];
    int pos = 0;
    char *var = NULL;

    if (!line)
        {return ( NULL);}
    while (line[i])
    {
        if ((line[i] == '\'' || line[i] == '"') && quote == 0)
        {
            pos = i;
            quote = line[i];
            continue;
        }
        
        if (line[i] == quote && i != pos)
            quote = 0;
        if (line[i] == '$' && (is_valid_char(line[i + 1]) || line[i + 1] == '?') && quote != '\'')
        {
            i++;
            if (ft_isdigit(line[i]) && is_valid_char(line[i + 1]))
            {
                i++;
                tmp[0] = line[i];
                tmp[1] = '\0';
                result = ft_strjoin_ws(result, tmp);
                i++;
                continue;
            }
            if (line[i] == '?')
            {
                result = ft_strjoin_ws(result, ft_itoa(glob_strct->ext_stat));
                i++;
                continue;
            }
            j = i;
            while (line[j] && is_valid_char(line[j]))
                j++;
            var = ft_strndup(&line[i], j - i);
            var_value = my_getenv2(var, glob_strct->env);
            if (var_value)
                result = ft_strjoin_ws(result, var_value);
            else
                result = ft_strjoin_ws(result, "");
            free(var);
            i = j;
        }
        else
        {
            tmp[0] = line[i];
            tmp[1] = '\0';
            result = ft_strjoin_ws(result, tmp);
            i++;
        }
    }
    return result;
}


char *expand_export(char *line, t_glob_st *glob_strct)
{
    if (!line)
        return (NULL);

    return (line);
}


void expand_env_vars(t_glob_st *glob_strct)
{
    t_cmd *current = glob_strct->cmd;
    char *expanded = NULL;
    int i = 0;
    while (current)
    {
        i = 0;

        if (current->line)
        {
            expanded = expand(current->line, glob_strct);
            current->line = expanded;
        }
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