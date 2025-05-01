#include "../parse.h"

int is_quote(char c)
{
    return (c == '\'' || c == '"');
}

int valid_quotes(char *s)
{
    int i;
    char    q;

    i = 0;
    while (s[i])
    {
        if (is_quote(s[i]))
        {
            q = s[i];
            i++;
            while (s[i]  && s[i] != q)
                i++;
            if (s[i] == '\0')
                return (0);
        }
        i++;
    }
    return (1);
}

void    skip_quotes(char *s, int *i, char q)
{
    (*i)++;
    while (s[*i] && s[*i] != q)
        (*i)++;
    if (s[*i] != '\0')
       ( *i)++;
}
