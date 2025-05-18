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
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
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


char *expand_one_var(char *line, t_env *env)
{
    char *varname = NULL;
    char *value = NULL;
    char *result = NULL;
    int i = 0;
    int expandable = 1;
    char q = 0;
    int start = 0;
    int end = 0;
    if (!line)
        return (line);
    if (line[0] && is_quote(line[0]))
    {
        q = line[0];
        if (q == '\'')
            expandable = 0;
        else
            expandable = 1;
        i++;
    }
    if (line[i] && line[i] == '$' && expandable) 
    {
        i++;
        start = i;
        end = start;
        // printf("line2: %s\n", line);
        while (line[end] && is_valid_char(line[end]) && !is_quote(line[end]) && !is_space(line[end]))
            end++;
        // printf("i: %d\n", i);
        varname = ft_strndup(&line[start], end - start);
        printf("varname: %s\n", varname);
        if (!varname)
            return (NULL);
        value = my_getenv2(varname, env);
        if (!value)
        {
            printf("value: %s\n", value);
            return (ft_strdup(line));
        }
        result = ft_strdup(value);
        printf("result: %s\n", result);
        free(varname);
        return (result);
    }
    return (line);
}

char *expand_var(char *line, t_env *env)
{
    char *result = NULL;
    // printf("line: %s\n", line);
    if (!line)
        return (NULL);
    
    if (!strchr(line, ' '))
    {
        result = expand_one_var(line, env);
        return (result);
    }

    char **parts = ft_split(line, ' ');
    if (!parts)
        return (NULL);
    int i = 0;
    while (parts[i])
    {
        parts[i] = expand_one_var(parts[i], env);
        i++;
    }
    
    i = 0;
    while (parts[i])
    {
        result = ft_strjoin(result, parts[i]);
        // printf("result: %s\n", result);
        i++;
    }
    free_arr(parts);
    

    return (result);
}


void expand_env_vars(t_cmd *cmd, t_env *env)
{
    t_cmd *current = cmd;
    char *expanded = NULL;
    while (current)
    {
        if (!is_export_or_unset(current->line))
        {
            expanded = expand_var(current->line, env);
            // printf("expanded: %s\n", expanded);
            // free(current->line);    \\ seg fault when freeing
            current->line = expanded;
            // printf("current: %s\n", current->line);
        }
        current = current->next;
    }
}