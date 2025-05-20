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

// char *expand_string(char *line, t_env *env)
// {
//     int i = 0;
//     char quote = 0;
//     char *result = malloc(1);
//     char *varname, *value, *tmp;

//     if (!line || !result)
//         return (NULL);

//     result[0] = '\0';

//     while (line[i])
//     {
        
//     }

//     return result;
// }


// char *expand_one_var(char *line, t_env *env)
// {
//     char *varname = NULL;
//     char *value = NULL;
//     char *result = NULL;
//     int i = 0;
//     int expandable = 1;
//     char q = 0;
//     int start = 0;
//     int end = 0;
//     char *prefix = NULL;

//     if (!line)
//         return (line);
//     if (line[0] && is_quote(line[0]))
//     {
//         q = line[0];
//         if (q == '\'')
//             expandable = 0;
//         else
//             expandable = 1;
//         i++;
//     }
//     start = i;
//     while (line[i] && line[i] != '$' && line[i] != q)
//         i++;
//     end  = i;
//     prefix = ft_strndup(&line[start], end - start);
//     if (line[i] && line[i] == '$' && expandable) 
//     {
//         i++;
//         if (line[i] >= '0' && line[i] <= '9')
//         {
//             i++;
//             result = expand_one_var(&line[i], env);
//             return (result);
//         }
//         start = i;
//         end = start;
//         while (line[end] && is_valid_char(line[end]) && !is_quote(line[end]) && !is_space(line[end]))
//             end++;
//         varname = ft_strndup(&line[start], end - start);
//         if (!varname)
//             return (NULL);
//         value = my_getenv2(varname, env);
//         result = ft_strdup(prefix);
//         result = ft_strjoin2(result, value);
//         result = ft_strjoin2(result, strdup(expand_one_var(&line[end], env)));
//         free(varname);
//         return (result);
//     }
//     else
//     {
//         result = ft_strndup(prefix, end - start);
//         free(prefix);
//         if (line[i] && line[i] == q)
//             i++;
//         if (line[i])
//             result = ft_strjoin2(result, expand_one_var(&line[i], env));
//         return (result);
//     }
//     return (line);
// }

// char *expand_var(char *line, t_env *env)
// {
//     char *result = NULL;
//     // printf("line: %s\n", line);
//     if (!line)
//         return (NULL);
    
//     if (!strchr(line, ' '))
//     {
//         result = expand_one_var(line, env);
//         return (result);
//     }

//     char **parts = ft_split(line, ' ');
//     if (!parts)
//         return (NULL);
//     int i = 0;
//     while (parts[i])
//     {
//         parts[i] = expand_one_var(parts[i], env);
//         i++;
//     }
    
//     i = 0;
//     while (parts[i])
//     {
//         result = ft_strjoin(result, parts[i]);
//         printf("result: %s\n", result);
//         i++;
//     }
//     free_arr(parts);
    

//     return (result);
// }

char *remove_and_expand(char *line, t_env *env)
{
    char *result = malloc(1);
    char *var_value = NULL;
    int i = 0, j;
    char quote = 0;
    result[0] = '\0';
    char tmp[2];
    char *var = NULL;

    if (!line)
        return ( NULL);
    while (line[i])
    {
        if ((line[i] == '\'' || line[i] == '"') && quote == 0)
        {
            quote = line[i++];
            continue;
        }
        if (line[i] == quote)
        {
            quote = 0;
            i++;
            continue;
        }
        if (line[i] == '$' && quote != '\'')
        {
            i++;
            if (ft_isdigit(line[i]))
            {
                i++;
                tmp[0] = line[i];
                tmp[1] = '\0';
                result = ft_strjoin_ws(result, tmp);
                i++;
                continue;
            }
            j = i;
            while (line[j] && is_valid_char(line[j]))
                j++;
            var = ft_strndup(&line[i], j - i);
            var_value = my_getenv2(var, env);
            result = ft_strjoin_ws(result, var_value ? var_value : "");
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
            expanded = remove_and_expand(current->line, env);

            // printf("expanded: %s\n", expanded);
            // free(current->line);    \\ seg fault when freeing
            current->line = expanded;
            // printf("current: %s\n", current->line);
        }
        while (current->files && current->files[i].filename)
        {
            current->files[i].filename = remove_and_expand(current->files[i].filename, env);
            i++;
        }
        current = current->next;
    }
}