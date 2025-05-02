#include "../parse.h"

size_t ft_strlen(const char *str)
{
    size_t len;

    if (!str)
        return (0);
    len = 0;
    while (str[len])
        len++;
    return (len);
}

char	*ft_strchr(const char *s, int c)
{
	char	*p;

	p = (char *)s;
	while (*p != '\0')
	{
		if (*p == (char )c)
			return (p);
		p++;
	}
	if ((char )c == '\0')
		return (p);
	return (NULL);
}

char *ft_stringcpy(char *dst, const char *src, size_t n)
{
    size_t i;

    if (!dst || !src)
        return (NULL);
    i = 0;
    while (i < n && src[i])
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
    return (dst);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*trimmed_str;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	end = ft_strlen(s1);
	while (end > start && ft_strchr(set, s1[end - 1]))
		end--;
	trimmed_str = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!trimmed_str)
		return (NULL);
	ft_stringcpy(trimmed_str, s1 + start, end - start);
	return (trimmed_str);
}