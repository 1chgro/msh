#include "../parse.h"

char *ft_strdup(char *str)
{
    char *dup;
    int i;
    size_t len;

    i = 0;
    len = 0;
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