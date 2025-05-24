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


t_cmd *msh_parse(t_env *env)
{
    char *line;
    t_cmd *cmd;
    t_token *tokens;

    line = NULL;
    cmd = NULL;
    line = read_line();
    if (line == NULL)
        return (NULL);
    tokens = lexer(line);
    if (!check_syntax_err(tokens))
        return (free_tokens(tokens), NULL);
    cmd = create_cmd(tokens, env);
    if (!cmd)
        return (free_tokens(tokens), NULL);
    print_tokens(tokens);
    print_cmd(cmd);
    return (cmd);
}



void msh_loop(char **envp)
{
    t_cmd *cmd = NULL;
    t_env *env = NULL;
    int status = 0;

    copie_env(&env, envp);
    msh_signals();
    while(1)
    {
        cmd = msh_parse(env);
        if (!cmd)
        {
            status = 0;
            update_node_value(get_exit(env), ft_itoa(status), 0);
        }
        else
        {
            status = msh_execute(cmd, env);
            update_node_value(get_exit(env), ft_itoa(status), 0);
            free_cmd(cmd);
            cmd = NULL;
        }
    }
    free_env(env);
}
