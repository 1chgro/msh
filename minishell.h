#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#define DEBUGG_CHECK write(2, "got to this point successfully\n", 32);

typedef enum
{
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND,
	HEREDOC
} t_redirection_type;

typedef struct s_red
{
	char 					*filename;
	int 					fd;
	t_redirection_type 		type;
} t_red;

typedef struct s_cmd
{
	char 			*line;
	char 			**argv;
	t_red			*files;
	struct s_cmd  	*next;
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
void free_arr(char **arr);

// tokenization functions
t_token		*tokenize(char *line);
void		add_token(t_token **head, t_token *new_token);
t_token		*new_token(char *value, t_token_type type);
t_token_type		get_token_type(char *value);
void		free_tokens(t_token *tokens);

// syntax
int check_syntax_err(t_token *tokens);

// expand_env functions


//cmd create functions
t_cmd *create_cmd(t_token *tokens, t_env *env);
int copie_env(t_env **c_env, char **env);


// Utility functions
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_stringcpy(char *dst, const char *src, size_t n);
size_t	ft_strlen(const char *str);
char	*ft_strdup(char *str);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strjoin_ws(char const *s1, char const *s2);
void	ft_putstr_fd(char *s, int fd);
char *ft_strndup(const char *s, int n);

int	is_quote(char c);
int	is_operator(char c);
int	is_space(char c);
int	is_redirection(t_token **token);
int	is_pipe(t_token **token);

//-------------------expanding-------------------//
void   expand_env_vars(t_cmd *cmd, t_env *env);


//--------------------------excution-------------------//
int    msh_execute(t_cmd *cmd, t_env *env);
void    run_cmd(t_cmd *cmd, t_env *env);
int     copie_env(t_env **c_env, char **env);
char    *ft_strchr(const char *s, int c);
char    **ft_split(char const *s, char c);
// int ft_strcmp(const char *s1, const char *s2);
t_env *create_node(char *key, char *value);
void append_node(t_env **head, t_env *node);
int  are_builtin(char    *cmd);
void	run_builtin(char **s_cmd, t_env *env);
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
void redirection(t_cmd *cmd);
char	*get_key(char	*str);
char	*get_value(char	*str);
char	*ft_strjoin_(char *s1, char *s2);
int	ft_isdigit(int c);
#endif