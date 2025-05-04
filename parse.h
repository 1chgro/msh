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


#define DEBUGG_CHECK write(2, "got to this point successfully\n", 32)

typedef struct s_cmd
{
	char          **argv;           // execve-style args
	char          *infile;          // for <
	char          *outfile;         // for > or >>
	int           append;           // 1 if >>, 0 if >
	int           has_heredoc;      // flag for heredoc, //! make sure there is less than 17 heredocs
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
	char			*value;
	t_token_type	type;
	struct s_token	*next;
} t_token;

void	msh_loop(void);
void	msh_signals(void);
int		valid_quotes(char *s);
void		skip_quotes(char *s, int *i, char q);
t_token		*lexer(char *line);

// tokenization functions
t_token		*tokenize(char *line);
void		add_token(t_token **head, t_token *new_token);
t_token		*new_token(char *value, t_token_type type);
t_token_type		get_token_type(char *value);
void		free_tokens(t_token *tokens);

// syntax
int check_syntax_err(t_token *tokens);

//cmd create functions
t_cmd *create_cmd(t_token *tokens);

// Utility functions
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_stringcpy(char *dst, const char *src, size_t n);
size_t	ft_strlen(const char *str);
char	*ft_strdup(char *str);
int		ft_strcmp(const char *s1, const char *s2);

int	is_quote(char c);
int	is_operator(char c);
int	is_space(char c);
int	is_redirection(t_token **token);
int	is_pipe(t_token **token);


#endif