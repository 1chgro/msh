#include "../minishell.h"



char *expand_export(char *line, t_glob_st *glob_strct)
{
    char *result = NULL;

    if (ft_strcmp(line, "export") == 0)
        return (line);
    if (line[0] == '$')
        result = expand(line, glob_strct);
    else
        result = expand(line, glob_strct);
    
    return (result);
}