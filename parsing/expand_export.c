#include "../minishell.h"

int check_kv_quotes(char *key)
{
    if (!key)
        return (0);
    
    int len = ft_strlen(key);
    if (len < 2)
        return (0);
    
    // Check if key starts and ends with single quotes
    if (key[0] == '\'' && key[len - 1] == '\'')
        return (1);
    
    // Check if key starts and ends with double quotes
    if (key[0] == '"' && key[len - 1] == '"')
        return (1);
    
    return (0);
}

char *add_quotes(char *value)
{
    char *result = NULL;
    if (!value)
        return (NULL);
    
    // Check if value already has quotes
    if (value[0] == '\'' && value[ft_strlen(value) - 1] == '\'')
    {
        result = ft_strdup(value);
    }
    else if (value[0] == '"' && value[ft_strlen(value) - 1] == '"')
    {
        result = ft_strdup(value);
    }
    else
    {
        result = ft_strjoin_ws("\"", value);
        result = ft_strjoin_ws(result, "\"");
    }
    return (result);
}


char *export_onebone(char **export_args, t_glob_st *glob_strct)
{
    char *result = NULL;
    char *var_name = NULL;
    char *var_value = NULL;
    char *expanded_value = NULL;
    int i = 1;
    
    if (!export_args || !glob_strct || !glob_strct->env)
        return (NULL);
    result = ft_strdup("export ");
    while (export_args[i])
    {
        int j = 0;
        while (export_args[i][j] && export_args[i][j] != '=')
            j++;
        char *original_key = ft_strndup(export_args[i], j);
        int key_quotes = check_kv_quotes(original_key);
        var_name = ft_strndup(export_args[i], j);
        result = ft_strjoin_ws(result, var_name);
        if (export_args[i][j] == '=')
        {
            result = ft_strjoin_ws(result, "=");
            j++;
            var_value = ft_strdup(&export_args[i][j]);
            if (key_quotes)
            {
                int has_dollar = 0;
                int v = 0;
                while (var_value && var_value[v])
                {
                    if (var_value[v] == '$')
                    {
                        has_dollar = 1;
                        break;
                    }
                    v++;
                }
                if (has_dollar)
                {
                    expanded_value = expand(var_value, glob_strct);
                    if (expanded_value && ft_strlen(expanded_value) > 0)
                    {
                        char **tokens = ft_split(expanded_value, ' ');
                        if (tokens && tokens[0])
                        {
                            result = ft_strjoin_ws(result, tokens[0]);
                            int k = 1;
                            while (tokens[k])
                            {
                                result = ft_strjoin_ws(result, " ");
                                result = ft_strjoin_ws(result, tokens[k]);
                                k++;
                            }
                        }
                        int k = 0;
                        while (tokens && tokens[k])
                        {
                            free(tokens[k]);
                            k++;
                        }
                        free(tokens);
                    }
                    if (expanded_value)
                    {
                        free(expanded_value);
                        expanded_value = NULL;
                    }
                }
                else
                    result = ft_strjoin_ws(result, var_value);
            }
            else
            {
                var_value = add_quotes(var_value);
                expanded_value = expand(var_value, glob_strct);
                if (expanded_value)
                {
                    result = ft_strjoin_ws(result, expanded_value);
                    free(expanded_value);
                    expanded_value = NULL;
                }
            }
        }
        if (original_key)
            free(original_key);
        if (var_name)
            free(var_name);
        if (var_value)
            free(var_value);
        i++;
        if (export_args[i])
            result = ft_strjoin_ws(result, " ");
    }
    return (result);
}

char *expand_export(char *line, t_glob_st *glob_strct)
{
    char *result = NULL;

    if (!line || !glob_strct || !glob_strct->env)
    {
        return (NULL);
    }
    char **export_args = split_line_to_args(line);
    if (!export_args)
        return (NULL);
    result = export_onebone(export_args, glob_strct);
    
    return (result);
}