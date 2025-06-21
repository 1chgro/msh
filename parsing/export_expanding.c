#include "../minishell.h"

int check_if_export(char *line, t_glob_st *glob_strct)
{
    if (!line)
        return (0);
    char *tmp = expand(line, glob_strct);
    tmp = remove_outer_quotes(tmp);
    tmp = ft_strtrim(tmp, " \t\n\r\v\f");
    if (ft_strncmp("export", tmp, 6) == 0)
        return (free(tmp), 1);
    return (0);
}

char **split_key_val(char *str)
{
    char **result;
    result = malloc(sizeof(char *) * 3);
    if (!result)
        return (NULL);
    char *equal_sign = ft_strchr(str, '=');
    if (equal_sign)
    {
        *equal_sign = '\0';
        result[0] = ft_strdup(str);
        result[1] = ft_strdup(equal_sign + 1);
        result[2] = NULL;
    }
    else
    {
        result[0] = ft_strdup(str);
        result[1] = NULL;
    }
    return (result);
}

int check_key(char *key)
{
    int i = 0;
    char quote = 0; 
    while (key[i])
    {
        if (is_quote(key[i]))
        {
            if (quote == 0)
                quote = key[i];
            while (key[i] && key[i] != quote)
                i++;
            if (key[i] == '\0')
                return (0);
            if (key[i] == quote)
                return (1);
        }
        i++;
    }
    return (0);
}

int check_value(char *value)
{
    if (!value)
        return (0);
    int i = 0;
    if (ft_strchr(value, '$'))
        return (1);
    return (0);
}

char *add_quotes(char *value)
{
    if (!value)
        return (NULL);
    char *result = NULL;
    if (value[0] == '"' || value[0] == '\'')
    {
        result = ft_strdup(value);
    }
    else
    {
        result = malloc(sizeof(char) * (ft_strlen(value) + 3));
        if (!result)
            return (NULL);
        result[0] = '"';
        ft_stringcpy(&result[1], value, ft_strlen(value));
        result[ft_strlen(value) + 1] = '"';
        result[ft_strlen(value) + 2] = '\0';
    }
    return (result);
}

char *expand_key_value(char *str, t_glob_st *glob_strct)
{
    char *result = NULL;
    char *key = NULL;
    char *value = NULL;
    char *equal_sign = ft_strchr(str, '=');
    char **key_val = split_key_val(str);
    if (!key_val)
        return (NULL);
    int i = 0;
    // while (key_val[i])
    // {
    //     printf("key_val[%d]: %s\n", i, key_val[i]);
    //     i++;
    // }
    int split_value = 0;
    if (key_val[0])
    {
        if (check_key(key_val[0]) == 1 && check_value(key_val[1]) == 1)
        {
            split_value = 1;
        }
        key = expand(key_val[0], glob_strct);
        if (!key)
            return (NULL);
    }
    char **value_arr = NULL;
    if (key_val[1])
    {
        value = expand(key_val[1], glob_strct);
        if (!split_value)
            value = add_quotes(value);
        else
        {
            value_arr = ft_split(value, ' ');
            free(value);
            value = NULL;
        }
    }
    int k = 0;
    while (value_arr && value_arr[k])
    {
        value = ft_strjoin(value, value_arr[k]);
        k++;
    }

    result = ft_strjoin(result, key);
    if (value)
    {
        result = ft_strjoin_ws(result, "=");
        result = ft_strjoin_ws(result, value);
    }
    // printf("split_value: %d\n", split_value);
    return (result);
}

char *expand_export(char *line, t_glob_st *glob_strct)
{
    (void)glob_strct;
    char *result = NULL;
    if (!line)
        return (NULL);
    char **arr = split_line_to_args(line);
    if (!arr)
        return (NULL);
    int i = 0;
    char **key_val = malloc(sizeof(char *) * 3);
    int j = 0;
    char *expanded = NULL;
    while (arr[i])
    {
        expanded = expand_key_value(arr[i], glob_strct);
        result = ft_strjoin(result, expanded);
        i++;
    }
    // printf("result export: %s\n", result);
    return (result);
}