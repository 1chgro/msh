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

typedef struct s_cmd
{
    char          **argv;           // execve-style args
    char          *infile;          // for <
    char          *outfile;         // for > or >>
    int           append;           // 1 if >>, 0 if >
    int           has_heredoc;      // flag for heredoc
    char          *heredoc_input;   // raw heredoc content (core only)
    struct s_cmd  *next;            // next command in pipeline
} t_cmd;


typedef enum
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_APPEND,
    TOKEN_HEREDOC,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
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
void    skip_quotes(char *s, int *i, char q);



// Utility functions
char	*ft_strtrim(char const *s1, char const *set);
char *ft_stringcpy(char *dst, const char *src, size_t n);
size_t ft_strlen(const char *str);
char *ft_strdup(char *str);
int is_quote(char c);

#endif