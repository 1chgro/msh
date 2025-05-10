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

void print_token(t_token *token)
{
    if (token == NULL)
        return;
    if (token->type == TOKEN_WORD)
        printf("WORD: %s\n", token->value);
    else if (token->type == TOKEN_PIPE)
        printf("PIPE: %s\n", token->value);
    else if (token->type == TOKEN_APPEND)
        printf("APPEND: %s\n", token->value);
    else if (token->type == TOKEN_HEREDOC)
        printf("HEREDOC: %s\n", token->value);
    else if (token->type == TOKEN_REDIRECT_IN)
        printf("REDIRECT_IN: %s\n", token->value);
    else if (token->type == TOKEN_REDIRECT_OUT)
        printf("REDIRECT_OUT: %s\n", token->value);
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

void print_cmd(t_cmd *cmd)
{
    printf("Command:\n");
    int i = 0;
    if (cmd->argv)
    {
        printf("Arguments:\n");
        while (i < 3)
            printf(" %s\n", cmd->argv[i++]);
    }
    printf("Input file:\n");
    if (cmd->infile)
    {
        i = 0;
        while (cmd->infile[i])
            printf(" %s\n", cmd->infile[i++]);
    }
    printf("Output file:\n");
    if (cmd->outfile)
    {
        i = 0;
        while (cmd->outfile[i])
            printf(" %s\n", cmd->outfile[i++]);
    }
    printf("Heredoc:\n");
    if (cmd->heredoc)
    {
        t_heredoc *tmp_h = cmd->heredoc;
        while (tmp_h)
        {
            printf("delimiter: %s\n", tmp_h->delimiter);
            printf("file_descriptor: %d\n", tmp_h->fd);
            tmp_h = tmp_h->next;
        }
    }
    printf("Append: %d\n", cmd->append);
    printf("next: %p\n", cmd->next);
    printf("---------------------\n");

}


void print_tokens(t_token *tokens, t_cmd *cmd, int flg)
{
    if (tokens == NULL)
        return;
    if (flg == 0)
    {
        while (tokens)
        {
            print_token(tokens);
            tokens = tokens->next;
        }
    }
    else if (flg == 1)
    {
        while(cmd)
        {
            print_cmd(cmd);
            cmd = cmd->next;
        }
    }
    
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
    // here we can expand the env variables
    expand_env(tokens, env);
    // create the cmd linekd list
    cmd = create_cmd(tokens);
    // print_tokens(tokens, cmd, 0);
    // print_cmd(cmd);
    return (cmd);
}

void msh_loop(char **envp)
{
    t_cmd *cmd = NULL;
    t_env *env = NULL;
    copie_env(&env, envp);
    // msh_signals();
    int status = 0;
    while(1)
    {
        cmd = msh_parse(env);
        status = msh_execute(cmd, env);
        free(cmd);
    }
}