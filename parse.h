#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>


typedef struct s_ast
{
    struct s_ast    *left;
    struct s_ast    *right;
    void            *data;
} t_ast;


typedef enum
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_APPEND,
    TOKEN_HEREDOC,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_OPEN_PAREN,
    TOKEN_CLOSE_PAREN,
    TOKEN_OR,
    TOKEN_AND,
} t_token_type;

typedef struct s_token {
    char *value;
    t_token_type type;
    struct s_token *next;
} t_token;

void    msh_loop(void);
void    msh_signals(void);
t_token *lexer(char *line);
void    free_tokens(t_token *tokens);
int valid_quotes(char *s);
int check_parenthesis(char *line);

#endif