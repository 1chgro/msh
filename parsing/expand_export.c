#include "../minishell.h"

int check_key(char *line)
{
    int len = ft_strlen(line);
    if (line[0] == '\'' && line[len - 1] == '\'')
        return (0);
    return (1);
}

int check_equal(char *line)
{
    int i = 0;
    while (line[i] && line[i] != '=')
        i++;
    if (line[i] == '=')
        return (1);
    return (0);
}

int count_key_value(char *line)
{
    int count = 0;
    int i = 0;
    int in_word = 0;
    
    while (line[i])
    {
        if (!is_space(line[i]))
        {
            if (!in_word)
            {
                count++;
                in_word = 1;
            }
        }
        else
        {
            in_word = 0;
        }
        i++;
    }
    return (count);
}

char **extract_key_value(char *line)
{
    int count = count_key_value(line);
    char **key_value = malloc(sizeof(char *) * (count + 1));
    if (!key_value)
        return (NULL);
    
    int i = 0;
    int j = 0;
    int start = 0;
    int in_word = 0;
    
    // Skip leading spaces
    while (line[i] && is_space(line[i]))
        i++;
    
    start = i;
    while (line[i] && j < count)
    {
        if (!is_space(line[i]))
        {
            if (!in_word)
            {
                start = i;
                in_word = 1;
            }
        }
        else if (in_word)
        {
            // End of current word
            key_value[j] = ft_strndup(&line[start], i - start);
            j++;
            in_word = 0;
            
            // Skip spaces to next word
            while (line[i] && is_space(line[i]))
                i++;
            i--; // Adjust for the increment at end of loop
        }
        i++;
    }
    
    // Handle last word if we ended in the middle of one
    if (in_word && j < count)
    {
        key_value[j] = ft_strndup(&line[start], i - start);
        j++;
    }
    
    key_value[j] = NULL;
    return (key_value);
}

void free_key_value_array(char **key_value)
{
    int i = 0;
    
    if (!key_value)
        return;
    
    while (key_value[i])
    {
        free(key_value[i]);
        i++;
    }
    free(key_value);
}

char *expand_export(char *line, t_glob_st *glob_strct)
{
    char *result = NULL;
    int i = 0;
    char *expanded = NULL;
    char **key_value_array = NULL;
    int kv_index = 0;
    
    if (!line || ft_strcmp(line, "export") == 0)
        return (line);
    
    // If the whole line starts with $, expand it directly
    if (line[0] == '$')
    {
        result = expand(line, glob_strct);
        return (result);
    }
    
    // Start with "export"
    result = ft_strdup("export");
    
    // Skip "export" word
    while (line[i] && !is_space(line[i]))
        i++;
    
    // Skip spaces after "export"
    while (line[i] && is_space(line[i]))
        i++;
    
    // If nothing after export, return just "export"
    if (!line[i])
        return (result);
    
    int start = i;
    
    // Extract all key-value pairs from the remaining line
    key_value_array = extract_key_value(&line[start]);
    if (!key_value_array)
        return (result);
    
    // Process each key-value pair
    kv_index = 0;
    while (key_value_array[kv_index])
    {
        // Expand the current key-value pair
        expanded = expand(key_value_array[kv_index], glob_strct);
        
        // Join with result
        char *temp = result;
        result = ft_strjoin(result, expanded);
        free(temp);
        
        // Free the expanded string
        if (expanded != key_value_array[kv_index])
            free(expanded);
        
        kv_index++;
    }
    
    // Clean up
    free_key_value_array(key_value_array);
    
    return (result);
}