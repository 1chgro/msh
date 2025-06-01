#include "../minishell.h"

// char *env_extract_value(char *line, t_env *env)
// {
    
// }

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
    if (value[0] == '"' && value[ft_strlen(value) - 1] == '"')
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
    {
        return (NULL);
    }
    
    result = ft_strdup("export ");
    
    while (export_args[i])
    {
        int j = 0;
        
        // Extract variable name (key)
        while (export_args[i][j] && export_args[i][j] != '=')
            j++;
        
        // Check if the ORIGINAL key part has quotes
        char *original_key = ft_strndup(export_args[i], j);
        int key_quotes = check_kv_quotes(original_key);
        
        // Extract the actual variable name (without quotes)
        var_name = ft_strndup(export_args[i], j);
        
        // Add the variable name to result (without quotes)
        result = ft_strjoin_ws(result, var_name);
        
        // If there's an assignment
        if (export_args[i][j] == '=')
        {
            result = ft_strjoin_ws(result, "=");
            j++; // Skip the '=' character
            
            // Get the value part
            var_value = ft_strdup(&export_args[i][j]);
            printf("keyquotes: %d, var_value: %s\n", key_quotes, var_value);
            if (key_quotes)
            {
                // Case: export 'h'=$a -> h="a" key: b key: c
                // When key has quotes, only split if the value contains $ expansion
                
                // Check if the original value contains $ for expansion
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
                    printf("Expanding variable: %s\n", var_value);
                    // Only expand and split if there's a $ variable
                    expanded_value = expand(var_value, glob_strct);
                    // expanded_value = remove_outer_quotes(expanded_value);
                    if (expanded_value && ft_strlen(expanded_value) > 0)
                    {
                        // Split expanded value on spaces and create separate exports
                        char **tokens = ft_split(expanded_value, ' ');
                        if (tokens && tokens[0])
                        {
                            // First token becomes the value for this variable
                            result = ft_strjoin_ws(result, "\"");
                            result = ft_strjoin_ws(result, tokens[0]);
                            result = ft_strjoin_ws(result, "\"");
                            
                            // Additional tokens become separate key declarations
                            int k = 1;
                            while (tokens[k])
                            {
                                result = ft_strjoin_ws(result, " ");
                                result = ft_strjoin_ws(result, tokens[k]);
                                // result = ft_strjoin_ws(result, "=00");
                                k++;
                            }
                        }
                        else
                        {
                            result = ft_strjoin_ws(result, "\x01\x01");
                        }
                        // Free tokens array
                        int k = 0;
                        while (tokens && tokens[k])
                        {
                            free(tokens[k]);
                            k++;
                        }
                        free(tokens);
                    }
                    else
                    {
                        result = ft_strjoin_ws(result, "\x01\x01");
                    }
                    
                    if (expanded_value)
                    {
                        free(expanded_value);
                        expanded_value = NULL;
                    }
                }
                else
                {
                    // No $ expansion, just use the value as-is (including quotes)
                    result = ft_strjoin_ws(result, var_value);
                }
            }
            else
            {
                // Case: export h=$a -> h="a b c"
                // When key has no quotes, expand the value and keep as single quoted string
                var_value = add_quotes(var_value);
                expanded_value = expand(var_value, glob_strct);
                if (expanded_value)
                {
                    result = ft_strjoin_ws(result, "\x01");
                    result = ft_strjoin_ws(result, expanded_value);
                    result = ft_strjoin_ws(result, "\x01");
                    free(expanded_value);
                    expanded_value = NULL;
                }
                else
                {
                    result = ft_strjoin_ws(result, "\x01\x01");
                }
            }
        }
        else
        {
            // No assignment, just check if variable exists in environment
            char *env_value = my_getenv2(var_name, glob_strct->env);
            if (env_value)
            {
                result = ft_strjoin_ws(result, "=\x01");
                result = ft_strjoin_ws(result, env_value);
                result = ft_strjoin_ws(result, "\x01");
            }
        }
        
        // Cleanup
        if (original_key)
            free(original_key);
        if (var_name)
            free(var_name);
        if (var_value)
            free(var_value);
        
        i++;
        
        // Add space between multiple exports if not the last one
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
    {
        return (NULL);
    }
    result = export_onebone(export_args, glob_strct);
    
    return (result);
}