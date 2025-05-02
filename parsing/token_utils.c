#include "../parse.h"

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return (*s1 - *s2);
}


t_token_type get_token_type(char *value)
{
    if (ft_strcmp(value, "|") == 0)
        return TOKEN_PIPE;
    else if (ft_strcmp(value, ">>") == 0)
        return TOKEN_APPEND;
    else if (ft_strcmp(value, "<<") == 0)
        return TOKEN_HEREDOC;
    else if (ft_strcmp(value, "<") == 0)
        return TOKEN_REDIRECT_IN;
    else if (ft_strcmp(value, ">") == 0)
        return TOKEN_REDIRECT_OUT;
    return TOKEN_WORD;
}

t_token *new_token(char *value, t_token_type type)
{
    t_token *token;

    if (!value || !*value)
        return (NULL);
    token = malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->value = ft_strdup(value);
    if (!token->value)
    {
        free(token);
        return (NULL);
    }
    token->type = type;
    token->next = NULL;
    return (token);
}

void add_token(t_token **head, t_token *new_token)
{
    t_token *tmp;

    if (!new_token)
        return;
    if (!*head)
    {
        *head = new_token;
        return;
    }
    tmp = *head;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new_token;
}

void free_tokens(t_token *tokens)
{
    t_token *tmp;

    while (tokens)
    {
        tmp = tokens;
        tokens = tokens->next;
        free(tmp->value);
        free(tmp);
        tmp = NULL;
    }
}