#include "../parse.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
    size_t  total_len;
    char	*jnd_str;
    size_t  i;
    size_t  j;

    if (!s1)
        return (ft_strdup((char *)s2));
    if (!s2)
        return (ft_strdup((char *)s1));
    (1) && (i = -1, j = 0);
    total_len = ft_strlen(s1) + ft_strlen(s2);
    jnd_str = malloc(sizeof(char) * (total_len + 2));
    if (!jnd_str)
        return (free((char *)s1), NULL);
    while(++i < ft_strlen(s1))
        jnd_str[i] = s1[i];
    jnd_str[i] = ' ';
    i++;
    while (i <= total_len)
        jnd_str[i++] = s2[j++];
    jnd_str[i] = '\0';
    free((char *)s1);
	return (jnd_str);
}
