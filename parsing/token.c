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
        return ;
    if (!*head)
    {
        *head = new_token;
        return ;
    }
    tmp = *head;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new_token;
}

int is_space(char c)
{
    return (c == ' ' || (c >= 9 && c <= 13));
}

int is_operator(char c)
{
    return (c == '|' || c == '>' || c == '<');
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


t_token *tokenize(char *line)
{
    t_token *tokens = NULL;
    char *word = NULL;
    int i = 0;
    int start = 0;
    int end = 0;
    char *op = NULL;
    int len = 0;

    while(line[i])
    {
        if (line[i] && is_space(line[i]))
        {
            i++;
            continue;
        }
        if (line[i] && is_operator(line[i]))
        {
            len = 1;
            if ((line[i] == '<' && line[i + 1] == '<')
            || (line[i] == '>' && line[i + 1] == '>'))
            {
                len = 2;
                op = malloc(len + 1);
                if (!op)
                {
                    free_tokens(tokens);
                    return (NULL);
                }
                ft_stringcpy(op, &line[i], len);
            }
            else
            {
                op = malloc(2);
                if (!op)
                {
                    free_tokens(tokens);
                    return (NULL);
                }
                op[0] = line[i];
                op[1] = '\0';
            }
            add_token(&tokens, new_token(op, get_token_type(op)));
            free(op);
            i += len;
            continue;
        }
        else if (line[i] && !is_space(line[i]) && !is_operator(line[i]))
        {
            start = i;
            if (line[i] == '\'' || line[i] == '\"')
            {
                char q = line[i];
                skip_quotes(line, &i, q);
                end = i;
            }
            else
            {
                while(line[i] && !is_space(line[i]) && !is_operator(line[i]))
                    i++;
            }
            end = i;
            word = malloc(end - start + 1);
            if (!word)
            {
                free_tokens(tokens);
                return (NULL);
            }
            ft_stringcpy(word, &line[start], end - start);
            word[end - start] = '\0';
            add_token(&tokens, new_token(word, TOKEN_WORD));
            free(word);
        }
    }
    return (tokens);
}


void remove_quotes(t_token **tokens)
{
    t_token *tmp = *tokens;

    while (tmp)
    {
        if (tmp->type == TOKEN_WORD)
        {
            char *new_value = ft_strtrim((char *)tmp->value, "\"\'");
            free(tmp->value);
            tmp->value = new_value;
        }
        tmp = tmp->next;
    }
}

t_token *lexer(char *line)
{
    t_token *tokens = NULL;

    if (!line || !*line)
        return (NULL);
    line = ft_strtrim(line, " \t\n\r\v\f");
    if (!line)
        return (NULL);
        printf("line: |%s|\n", line);
    if (!valid_quotes(line))
        return (free(line), write(2, "Error: Quotes err\n", 13), NULL);
    tokens = tokenize(line);
    if (!tokens)
        return (free(line), NULL);
    //! this should be done int the expanding part this is just a test for now
    remove_quotes(&tokens);
    free(line);
    return (tokens);
}