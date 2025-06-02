#include "../minishell.h"

int	valid_quotes(char *s)
{
	int		i;
	char	q;

	i = 0;
	while (s[i])
	{
		if (is_quote(s[i]))
		{
			q = s[i];
			i++;
			while (s[i] && s[i] != q)
				i++;
			if (s[i] == '\0')
				return (0);
		}
		i++;
	}
	return (1);
}

void	skip_quotes(char *s, int *i, char q)
{
	(*i)++;
	while (s[*i] && s[*i] != q)
		(*i)++;
	if (s[*i] != '\0')
		(*i)++;
}

// char *remove_outer_quotes(char *s)
// {
//     int i = 0, j = 0;
//     char quote = 0;
//     char output[ft_strlen(s) + 1];

// 	if (!s)
// 		return (NULL);
//     while (s[i])
// 	{
// 		printf("s[%d] = %c\n", i, s[i]);
//         if (!quote && (s[i] == '\"' || s[i] == '\'')) {
//             quote = s[i];
//             i++;
//             continue;
//         }
//         if (quote && s[i] == quote) {
//             quote = 0;
//             i++;
//             continue;
//         }
//         output[j++] = s[i++];
//     }
//     output[j] = '\0'; 
//     return (ft_strdup(output));
// }

char *remove_outer_quotes(char *s)
{
    int i = 0, j = 0;
    char quote = 0;
    char *output;
    int len;
    
    if (!s)
        return (NULL);
    
    len = ft_strlen(s);
    output = malloc(len + 1);
    if (!output)
        return (NULL);
    
    while (s[i])
    {
        if (!quote && (s[i] == '"' || s[i] == '\''))
        {
            // Starting a quoted section
            quote = s[i];
            i++;
            continue;
        }
        else if (quote && s[i] == quote)
        {
            // Ending the quoted section
            quote = 0;
            i++;
            continue;
        }
        else if (quote == '"' && s[i] == '\'')
        {
            // Single quote inside double quotes - preserve it
            output[j++] = s[i++];
        }
        else if (quote == '\'' && s[i] == '"')
        {
            // Double quote inside single quotes - preserve it
            output[j++] = s[i++];
        }
        else
        {
            // Regular character or quote that should be preserved
            output[j++] = s[i++];
        }
    }
    
    output[j] = '\0';
    return (output);
}


char **remove_quotes_arr(char **argv)
{
	int i;
	int j;
	char **new_argv;
	int len;

	(1) && (i = 0, j = 0, new_argv = NULL, len = 0);
	if (!argv)
		return (NULL);
	while (argv[len++])
	new_argv = malloc(sizeof(char *) * (len + 1));
	if (!new_argv)
		return (free(argv), NULL);
	while (argv[i])
	{
		new_argv[i] = remove_outer_quotes(argv[i]);
		free(argv[i]);
		if (!new_argv)
		{
			while (--j >= 0)
				free(new_argv[j]);
			return (free(new_argv), NULL);
		}
		i++;
	}
	return (new_argv[i] = NULL, new_argv);
}
