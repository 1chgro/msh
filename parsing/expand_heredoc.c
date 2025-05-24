#include "../minishell.h"

char *expand_heredoc(char *line, t_env *env)
{
    char *result = malloc(1);
    char *var_value = NULL;
    int i = 0, j;
    result[0] = '\0';
    char tmp[2];
    char *var = NULL;

    if (!line)
        {return ( NULL);}
    while (line[i])
    {
        if (line[i] == '$' && (is_valid_char(line[i + 1]) || line[i + 1] == '?'))
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
    return (result);
}