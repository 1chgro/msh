#include "../minishell.h"

char	*ft_strdup(char *str)
{
	size_t	len;
	char	*dup;
	int		i;

	i = 0;
	len = 0;
	if (!str)
		return (NULL);
	while (str[len])
		len++;
	dup = malloc(sizeof(char) * (len + 1));
	if (!dup)
		return (NULL);
	while (str[i])
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*ft_strjoin_ws(char const *s1, char const *s2)
{
	char		*jnd_str;
	size_t		total_len;
	size_t		i;
	size_t		j;

	if (!s1)
		return (ft_strdup((char *)s2));
	if (!s2)
		return (ft_strdup((char *)s1));
	(1) && (i = -1, j = 0);
	total_len = ft_strlen(s1) + ft_strlen(s2);
	jnd_str = malloc(sizeof(char) * (total_len + 1));
	if (!jnd_str)
		return (free((char *)s1), NULL);
	while (++i < ft_strlen(s1))
		jnd_str[i] = s1[i];
	while (s2[j] && i < total_len)
		jnd_str[i++] = s2[j++];
	jnd_str[i] = '\0';
	if (s1)
		free((char *)s1);
	return (jnd_str);
}

void	ft_putstr_fd(char *s, int fd)
{
	size_t	i;

	if (!s)
		return ;
	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}
