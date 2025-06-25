#include "../minishell.h"

int check_if_export(char *line, t_glob_st *glob_strct)
{
    char *tmp;
    char *temp;
    if (!line)
        return (0);
    
    tmp = expand(line, glob_strct);
    if (!tmp)
        return (0);
    tmp = remove_outer_quotes(tmp);
    if (!tmp)
        return (0);
    tmp = ft_strtrim(tmp, " \t\n\r\v\f");
    if (!tmp)
        return (0);
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

char *expand_key_value(char *str, t_glob_st *glob_strct, int split_all_values)
{
    char *result = NULL;
    char *key = NULL;
    char *value = NULL;
    char *equal_sign = ft_strchr(str, '=');
    char **key_val = split_key_val(str);
    if (!key_val)
        return (NULL);
    int i = 0;

    int split_value = 0;
    if (key_val[0])
    {
        if (split_all_values || (check_key(key_val[0]) == 1 && check_value(key_val[1]) == 1))
            split_value = 1;
        key = expand(key_val[0], glob_strct);
        if (!key)
            return (NULL);
    }
    char **value_arr = NULL;
    if (key_val[1])
    {
        value = expand(key_val[1], glob_strct);
        if (!split_value)
        {
            value = remove_outer_quotes(value);
            value = add_quotes(value);
        }
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
    return (result);
}

char *get_word(char *line)
{
    char *word;
    int i = 0;
    
    while (line[i])
    {
        if (is_space(line[i]))
            break;
        i++;
    }
    word = ft_strndup(line, i);
    return (word);
}

int is_quoted_export(char *line)
{
    int quote;
    char *export_candidate;
	int j;
	if (!line)
		return (0);

    quote = 0;
    j = 0;
    export_candidate = get_word(line);
    if (ft_strcmp(export_candidate, "export") == 0)
        return (0);
	while (export_candidate[j])
	{
		if (export_candidate[j] == '\'' || export_candidate[j] == '"')
			quote++;
        j++;
	}
	return (quote);
}


char *expand_export(char *line, t_glob_st *glob_strct)
{
    char *result;
    char **arr;
    int i;
    int split_all_values;
    char *expanded;
    char *temp;

    if (!line)
        return (NULL);
    i = 0;
    expanded = NULL;
    arr = split_line_to_args(line);
    result = NULL;
    if (!arr)
        return (NULL);
    split_all_values = is_quoted_export(line);
    while (arr[i])
    {
        expanded = expand_key_value(arr[i], glob_strct, split_all_values);
        temp = result;
        result = ft_strjoin(temp, expanded);
        free(temp);
        i++;
    }
    return (result);
}
