#include "../parse.h"




int valid_quotes(char *s)
{
    int i;
    char    q;

    i = 0;
    while (s[i])
    {
        if (s[i] == '\'' || s[i] == '"')
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

void    skip_quotes(char *s, int *i, char q)
{
    *i += 1;
    while (s[*i] && s[*i] != q)
        *i += 1;
    if (s[*i] != '\0')
        *i +=1;
}

int check_parenthesis(char *line)
{
    int i = 0;
    int count = 0;
    while(line[i])
    {
        if (line[i] == '\'' || line[i] == '"')
            skip_quotes(line, &i, line[i]);
        if (line[i] == '(')
            count++;
        else if (line[i] == ')')
            count--;
        i++;
    }
    return (count == 0);
}
