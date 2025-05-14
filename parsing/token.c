#include "../minishell.h"

int handle_operator(char *line, int *i, t_token **tokens)
{
    char *op;
    int len;
    
    len = 1;
    if ((line[*i] == '<' && line[*i + 1] == '<') ||
        (line[*i] == '>' && line[*i + 1] == '>'))
        len = 2;
    op = malloc(len + 1);
    if (!op)
        return (0);
    ft_stringcpy(op, &line[*i], len);
    add_token(tokens, new_token(op, get_token_type(op)));
    free(op);
    *i += len;
    return (1);
}


int handle_word(char *line, int *i, t_token **tokens)
{
    int start;
    int end;
    char *word;
    char q;

    start = *i;
    while (line[*i])
    {
        if (is_quote(line[*i]))
        {
            q = line[*i];
            skip_quotes(line, i, q);
        }
        else if (!is_space(line[*i]) && !is_operator(line[*i]))
            (*i)++;
        else
            break;
    }
    (1) && (end = *i, word = malloc(end - start + 1));
    if (!word)
        return (0);
    ft_stringcpy(word, &line[start], end - start);
    add_token(tokens, new_token(word, TOKEN_WORD));
    return (free(word), 1);
}


t_token *tokenize(char *line)
{
    t_token *tokens;
    int i;
    
    tokens = NULL;
    i = 0;
    while (line[i])
    {
        if (is_space(line[i]))
        {
            i++;
            continue;
        }
        if (is_operator(line[i]))
        {
            if (!handle_operator(line, &i, &tokens))
                return (free_tokens(tokens), NULL);
        }
        else
        {
                if (!handle_word(line, &i, &tokens))
                    return (free_tokens(tokens), NULL);
        }
    }
    return (tokens);
}
