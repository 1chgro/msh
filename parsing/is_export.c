#include <stdlib.h>

int ft_strncmp(const char *s1, const char *s2, size_t n);
int ft_strlen(const char *s);

// Check if quotes are balanced
static int quotes_balanced(const char *str)
{
	int i = 0;
	char quote = '\0';

	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"'))
		{
			if (quote == '\0')
				quote = str[i];         // opening quote
			else if (str[i] == quote)
				quote = '\0';           // closing quote
		}
		i++;
	}
	return (quote == '\0'); // true if no unclosed quote left
}

// Remove all quote characters
static char *strip_quotes(const char *str)
{
	int i = 0, j = 0;
	char *out = malloc(ft_strlen(str) + 1);
	if (!out)
		return NULL;
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '"')
			out[j++] = str[i];
		i++;
	}
	out[j] = '\0';
	return out;
}

int is_export(char *line)
{
	char *clean;
	int result = 0;

	if (!line || !quotes_balanced(line))
		return (0);
	clean = strip_quotes(line);
	if (!clean)
		return (0);
	if (ft_strncmp(clean, "export", 6) == 0 && ft_strlen(clean) == 6)
		result = 1;
	free(clean);
	return (result);
}


char *fix_cmd(char *cmd)
{
    if (!cmd)
        return NULL;
	char *clean;
	if (!cmd || !quotes_balanced(cmd))
		return (0);
	clean = strip_quotes(cmd);
	if (!clean)
		return (0);
	return (clean);
}
