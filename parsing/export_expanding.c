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


char *expand_export(char *line, t_glob_st *glob_strct)
{
    (void)glob_strct;
    char *result;
    if (!line)
        return (NULL);
    char **arr = split_line_to_args(line);
    if (!arr)
        return (NULL);
    int i = 0;
    while (arr[i])
    {
        arr[i] = expand(arr[i], glob_strct);
        printf("arr[%d]: %s\n", i, arr[i]);
        i++;
    }
    result = malloc(1);
    // int i = 0;
    // while (arr[i])
    // {
    //     arr[i]
    // }
    
    
    return (line);
}



