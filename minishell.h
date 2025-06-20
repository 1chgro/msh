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
#include <sys/stat.h>
#include <limits.h>
#include <termios.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/ioctl.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif


extern int g_in_heredoc;

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

typedef struct s_glob_st
{
	t_env *env;
	t_cmd *cmd;
	t_token *tokens;
	int ext_stat;
    struct  termios origin;
} t_glob_st;


int check_if_export(char *line, t_glob_st *glob_strct);
char *expand_export(char *line, t_glob_st *glob_strct);

//----------------- print functions--------///
void   print_cmd(t_cmd *cmd);
void print_tokens(t_token *tokens);


//--------------- main functions -----------------------//
int	msh_loop(char **envp);
int   	msh_execute(t_glob_st *glob_strct);
void	msh_signals();
t_token		*lexer(char *line);



//------------------- free functions -----------------------//
void		free_arr(char **arr);
void		free_cmd(t_cmd *cmd);
void		free_tokens(t_token *tokens);

//------------------------ tokenization ---------------//
t_token				*tokenize(char *line);
t_token				*new_token(char *value, t_token_type type);
int				add_token(t_token **head, t_token *new_token);
t_token_type		get_token_type(char *value);


//------------------------- syntax -----------------------//
int check_syntax_err(t_glob_st *glob_strct);

//--------------------- cmd create ----------------------------//
t_cmd	*create_cmd(t_glob_st *glob_strct);
char **split_line_to_args(char *line);

//------------------- env --------------------------------------//
int		copie_env(t_env **c_env, char **env);
char	*my_getenv2(char *name, t_env *env);
int is_export(char *line);

//-----------------------Utility functions-----------------//
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_stringcpy(char *dst, const char *src, size_t n);
char	*ft_strdup(char *str);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strjoin_ws(char const *s1, char const *s2);
char	*ft_strndup(const char *s, int n);
int		ft_strcmp(const char *s1, const char *s2);
void	ft_putstr_fd(char *s, int fd);
size_t	ft_strlen(const char *str);

//-------------------is functions-------------------//
int	is_quote(char c);
int	is_operator(char c);
int	is_space(char c);
int	is_redirection(t_token **token);
int	is_pipe(t_token **token);
int	is_valid_char(char c);
int is_export(char *line);

//------------------- quotes -----------------------//
void	skip_quotes(char *s, int *i, char q);
char	*remove_outer_quotes(char *s);
int		valid_quotes(char *s);
char	**remove_quotes_arr(char **argv);

//------------------- expanding -------------------//
void	expand_env_vars(t_glob_st *glob_strct);
char	*expand(char *line, t_glob_st *glob_strct);
char	*expand_heredoc(char *line, t_glob_st *glob_strct);
char	*expand_export(char *line, t_glob_st *glob_strct);
char *fix_cmd(char *cmd);
//-------------------------- excution -------------------//
int		run_cmd(t_cmd *cmd, t_env *env, int last_ex);
int     copie_env(t_env **c_env, char **env);
char    *ft_strchr(const char *s, int c);
char    **ft_split(char const *s, char c);
t_env	*create_node(char *key, char *value, int have_equal);
void	append_node(t_env **head, t_env *node);
int		are_builtin(char    *cmd);
int		run_builtin(char **s_cmd, t_env **env, int last_ex);
int    ft_pwd(t_env *env);
int    ft_env(char **s_cmd, t_env *env);
int    ft_exit(char **s_cmd, int last_ex);
int    ft_export(char **s_cmd, t_env **env);
int    ft_unset(char **s_cmd, t_env **env);
long	ft_atoi(const char *str);
int    ft_echo(char **s_cmd);
int    ft_cd(char  **s_cmd, t_env  **env);
void  free_split(char **split);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
int redirection(t_cmd *cmd);
char	*get_key(char	*str);
char	*get_value(char	*str);
char	*ft_strjoin_(char *s1, char *s2);
int	ft_isdigit(int c);
int	here_doc(char *limiter, int *fd, t_glob_st *glob_strct);
char	*ft_substr(char *s, unsigned int index, size_t bytes);
char	*get_next_line(int fd);
char	*ft_itoa(int n);
void update_node_value(t_env *node, char *value, int append, int have_equal);
void	ft_putendl_fd(char *s, int fd);
void	ft_putchar_fd(char c, int fd);
char	*my_getenv(char *name, t_env *env);
void	free_env(t_env *env);
int open_heredoc(t_glob_st *glob_strct);
void close_heredoc(t_glob_st *glob_strct);
char *get_current_pwd(void);
char *take_store_pwd(char *path);
long ft_atoi_(const char *str, int *is_valid);
void get_terminall(struct termios *termios);
void set_terminall(struct termios *termios);
#endif