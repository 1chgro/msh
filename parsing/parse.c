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

void print_tokens(t_token *tokens)
{
    t_token *current = tokens;

    while (current)
    {
        print_token(current);
        current = current->next;
    }
}

void   print_cmd(t_cmd *cmd)
{
    t_cmd *current = cmd;
    while (current)
    {
        if (current->line)
            printf("line: %s\n", current->line);
        if (current->argv)
        {
            printf("argv: \n");
            for (int i = 0; current->argv[i]; i++)
                printf("\t\t%s \n", current->argv[i]);
        }
        if (current->files)
        {
            printf("files: ");
            for (int i = 0; current->files[i].filename; i++)
                printf("filename: %s\t |type: %u\t|fd: %d\n", current->files[i].filename, current->files[i].type, current->files[i].fd);
        }
        printf("---------------------\n");
        current = current->next;
    }
}

t_cmd *msh_parse(t_env *env)
{
    char *line;
    t_cmd *cmd;
    t_token *tokens;
    // (void)env;

    line = NULL;
    cmd = NULL;
    line = read_line();
    if (line == NULL)
        return (NULL);
    tokens = lexer(line);
    if (!check_syntax_err(tokens))
        return (free_tokens(tokens), NULL);
    // create the cmd linekd list
    cmd = create_cmd(tokens, env);
    if (!cmd)
        return (free_tokens(tokens), NULL);
    
    
    // if (is_export_or_unset(cmd->line))
    print_tokens(tokens);
    print_cmd(cmd);
    return (cmd);
}
void free_arr(char **arr)
{
    int i = 0;
    if (!arr)
        return ;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

void free_cmd_files(t_red *files)
{
    int i = 0;
    if (!files)
        return ;
    while (files[i].filename)
    {
        free(files[i].filename);
        i++;
    }
    free(files);
}

void free_cmd(t_cmd *cmd)
{
    t_cmd *temp;
    while (cmd)
    {
        free(cmd->line);
        free_arr(cmd->argv);
        free_cmd_files(cmd->files);
        temp = cmd;
        cmd = cmd->next;
        free(temp);
    }
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