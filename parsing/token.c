#include "../parse.h"

char *ft_strdup(char *str)
{
    char *dup;
    int i;
    int len;

    i = 0;
    len = 0;
    while(str[len])
        len++;
    dup = malloc(sizeof(char) * (len + 1));
    if (!dup)
        return (NULL);
    while(str[i])
    {
        dup[i] = str[i];
        i++;
    }
    dup[i] = '\0';
    return (dup);
}


t_token *new_token(char *value, t_token_type type)
{
    t_token *token;

    token = malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->value = ft_strdup(value);
    token->type = type;
    token->next = NULL;
    return (token);
}

void add_token(t_token **head, t_token *new_token)
{
    t_token *tmp;
    if (*head || !new_token)
        return;
    if (!*head)
        *head = new_token;
    else
    {
        tmp = *head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new_token;
    }
}

int is_space(char c)
{
    return (c == ' ' || (c >= 9 && c <= 13));
}

char *trim_line(char *line)
{
    char *trimmed;
    int start = 0;
    int end = 0;
    int i = 0;
    while(line[i] && is_space(line[i]))
        i++;
    start = i;
    while(line[i] && !is_space(line[i]))
        i++;
    end = i;
    trimmed = malloc(sizeof(char) * (end - start + 1));
    if (!trimmed)
        return (free(line), NULL);
    i = 0;
    while (start < end)
    {
        trimmed[i] = line[start];
        i++;
        start++;
    }
    trimmed[i] = '\0';
    return (trimmed);
}

t_token *tokenizer(char *line)
{
    t_token *token = NULL;
    line = trim_line(line);

    return (token);
}