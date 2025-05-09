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

typedef struct s_heredoc
{
	
	struct s_heredoc *prev;	// next heredoc
	char *delimiter;	// delimiter for heredoc
	int fd;
	struct s_heredoc *next;	// next heredoc
} t_heredoc;

typedef struct s_cmd
{
	char          **argv;       	// execve-style args
	char          **infile;          // for <
	char          **outfile;         // for > or >>
	int           append;           // 1 if >>, 0 if >
	// int           has_heredoc;   // flag for heredoc, //! make sure there is less than 17 heredocs
	t_heredoc	*heredoc;      		// for heredoc
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

typedef struct s_env{
    char    *key;
    char    *value;
    int     flag;
    int     index;
    struct s_env *next;
} t_env;

void	msh_loop(char **envp);
void	msh_signals();
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

// expand_env functions
t_token *expand_env(t_token *tokens, t_env *env);

//cmd create functions
t_cmd *create_cmd(t_token *tokens);
int copie_env(t_env **c_env, char **env);


// Utility functions
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_stringcpy(char *dst, const char *src, size_t n);
size_t	ft_strlen(const char *str);
char	*ft_strdup(char *str);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strjoin(char const *s1, char const *s2);
char	**ft_split(char const *s, char c);

int	is_quote(char c);
int	is_operator(char c);
int	is_space(char c);
int	is_redirection(t_token **token);
int	is_pipe(t_token **token);

//--------------------------excution-------------------//
int    msh_execute(t_cmd *cmd, t_env *env);
void    run_cmd(t_cmd *cmd, t_env *env);
int     copie_env(t_env **c_env, char **env);
char    *ft_strchr(const char *s, int c);
char    **ft_split(char const *s, char c);
// int ft_strcmp(const char *s1, const char *s2);
t_env *create_node(char **split_env);
void append_node(t_env **head, t_env *node);
void    ft_pwd();
void    ft_env(char **s_cmd, t_env *env);
void    ft_exit(char **s_cmd);
void    ft_export(char **s_cmd, t_env **env);
void    ft_unset(char **s_cmd, t_env **env);
long	ft_atoi(const char *str);
void    ft_echo(char **s_cmd);
void    ft_cd(char  **s_cmd, t_env  **env);
void free_split(char **split);
int	ft_strncmp(const char *s1, const char *s2, size_t n);

#endif