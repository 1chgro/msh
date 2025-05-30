#include "../minishell.h"

char *read_line(void)
{
	char *input;

	input = NULL;
	input = readline("$ ");
	if (!input) // hna ila kan ctrl +d 
	{
		rl_clear_history(); // kanmsh l history
        write(2, "exit\n", 5);
        // ltht atkon ft_exit blast exit
		exit(0);
	}
	else if (*input)
		add_history(input); // Add input to history
	else
		return (NULL); // hna ila kan input empty kan3awd nakhd input
	return (input);
}

t_token *lexer(char *line)
{
    t_token *tokens = NULL;

    if (!line || !*line)
        return (NULL);
    line = ft_strtrim(line, " \t\n\r\v\f");
    if (!line)
        return (NULL);
    if (!valid_quotes(line))
    {
        free(line);
        write(2, "msh: syntax error near unexpected token `quotes'\n", 50);
        return (NULL);
    };
    tokens = tokenize(line);
    if (!tokens)
        return (free(line), NULL);
    free(line);
    return (tokens);
}


t_cmd *msh_parse(t_glob_st *glob_strct)
{
    char *line;

    line = NULL;
    line = read_line();
    if (line == NULL)
        return (NULL);
    glob_strct->tokens = lexer(line);
    if (!check_syntax_err(glob_strct))
        return (free_tokens(glob_strct->tokens), glob_strct->ext_stat = 2, NULL);
    glob_strct->cmd = create_cmd(glob_strct);
    if (!glob_strct->cmd)
        return (free_tokens(glob_strct->tokens), NULL);
    print_tokens(glob_strct->tokens);
    print_cmd(glob_strct->cmd);
    return (glob_strct->cmd);
}

t_glob_st *init_glob_strct()
{
    t_glob_st *glob_strct = NULL;
    glob_strct = malloc(sizeof(t_glob_st));
    if (!glob_strct)
        return (NULL);
    glob_strct->cmd = NULL;
    glob_strct->env = NULL;
    glob_strct->tokens = NULL;
    glob_strct->ext_stat = 0;
    return (glob_strct);
}

void msh_loop(char **envp)
{
    // t_cmd *cmd = NULL;
    // t_env *env = NULL;
    t_glob_st *glob_strct = NULL;
    // int status = 0;

    glob_strct = init_glob_strct();
    copie_env(&glob_strct->env, envp);
    msh_signals();
    while(1)
    {
        glob_strct->cmd = msh_parse(glob_strct);
        if (open_heredoc(glob_strct))
        {
            close_heredoc(glob_strct);
            free_cmd(glob_strct->cmd);
            continue;
        }
        if (glob_strct->cmd)
        {
            glob_strct->ext_stat = msh_execute(glob_strct);
            free_cmd(glob_strct->cmd);
            glob_strct->cmd = NULL;
        }
    }
    free_env(glob_strct->env);
}
