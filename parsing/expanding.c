#include "../minishell.h"

int is_export_or_unset(char *line)
{
    if (!line)
        return (0);
    if (ft_strncmp(line, "export", 6) == 0 || ft_strncmp(line, "unset", 5) == 0)
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
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')|| (c == '_');
}

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}


char *expand(char *line, t_env *env)
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
                var_value = my_getenv2("?", env);
                if (var_value)
                    result = ft_strjoin_ws(result, var_value);
                else
                    result = ft_strjoin_ws(result, "");
                free(var);
                i++;
                continue;
            }
            j = i;
            while (line[j] && is_valid_char(line[j]))
                j++;
            var = ft_strndup(&line[i], j - i);
            var_value = my_getenv2(var, env);
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



void expand_env_vars(t_cmd *cmd, t_env *env)
{
    t_cmd *current = cmd;
    char *expanded = NULL;
    int i = 0;
    while (current)
    {
        i = 0;
        if (current->line && !is_export_or_unset(current->line))
        {
            // expanded = expand_var(current->line, env);
            expanded = expand(current->line, env);

            // printf("expanded: %s\n", expanded);
            // free(current->line);    \\ seg fault when freeing
            current->line = expanded;
            // printf("current: %s\n", current->line);
        }
        while (current->files && current->files[i].filename)
        {
            current->files[i].filename = expand(current->files[i].filename, env);
            current->files[i].filename = remove_outer_quotes(current->files[i].filename);
            i++;
        }
        current = current->next;
    }
}