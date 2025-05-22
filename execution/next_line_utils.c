#include "../minishell.h"

char	*ft_substr(char *s, unsigned int index, size_t bytes)
{
	size_t	i;
	char	*str;

	i = 0;
	if (!s)
		return (NULL);
	if (index > ft_strlen(s))
		return (ft_strdup(""));
	if (bytes > ft_strlen(s) - index)
		bytes = ft_strlen(s) - index;
	str = malloc(bytes + 1);
	if (!str)
		return (NULL);
	while (bytes--)
	{
		str[i++] = s[index++];
	}
	str[i] = '\0';
	return (str);
}