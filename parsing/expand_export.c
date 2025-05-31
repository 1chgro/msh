#include "../minishell.h"

// char *env_extract_value(char *line, t_env *env)
// {
    
// }
int check_kv_quotes(char *kv)
{
    int i = 0;
    int in_quotes = 0;
    char quote_char = '\0';

    while (kv[i])
    {
        if (is_quote(kv[i]))
        {
            if (!in_quotes)
            {
                quote_char = kv[i];
                in_quotes = 1;
            }
            else if (kv[i] == quote_char)
                in_quotes = 0;
        }
        i++;
    }
    return (in_quotes);
}

char *export_onebone(char **export_args, t_glob_st *glob_strct)
{
    char *result = NULL;
    char *var_name = NULL;
    char *var_value = NULL;
    int i = 1;
    if (!export_args || !glob_strct || !glob_strct->env)
    {
        return (NULL);
    }
    int j = 0;
    result = ft_strdup("export ");
    while (export_args[i])
    {
        int start = j;
        while(export_args[i][j] && export_args[i][j] != '=')
            j++;
        var_name = ft_strndup(export_args[i], j - start);
        // if there is wuotes is in the key name, the value should be expnaded and not be inside quotes
        // but if the quotes isnt in the key name, the   value should be inside quotes we are the ones to add them
        int key_quotes = check_kv_quotes(var_name);
        int value_quotes = check_kv_quotes(var_name);
        char *key_value = NULL;
        if (!key_quotes && !value_quotes)
        {
            if ()
            key_value = my_getenv2(var_name, glob_strct->env);
            if (export_args[i][j] == '=')
            {
                j++;
                var_name = ft_strjoin_ws(var_name, "=");
            }
            result = ft_strjoin_ws(result, var_name);
            // if key_value
            if (key_value)
            {
                result = ft_strjoin_ws(result, "\"");
                result = ft_strjoin_ws(result, key_value);
                result = ft_strjoin_ws(result, "\"");
            }
            else
            {
                result = ft_strjoin_ws(result, "\0");
            }
        }
        i++;
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
    {
        return (NULL);
    }
    result = export_onebone(export_args, glob_strct);
    
    return (result);
}