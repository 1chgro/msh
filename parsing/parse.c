#include "../parse.h"

char *read_line(void)
{
	char *input;

	input = NULL;
	input = readline("$ ");
	if (!input) // hna ila kan ctrl +d 
	{
		rl_clear_history(); // kanmsh l history
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

t_cmd *msh_parse(void)
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
    t_token *tmp   = tokens;
    while (tmp)
    {
        print_token(tmp);
        tmp = tmp->next;
    }
    return (cmd);
}

void msh_loop(void)
{
    t_cmd *cmd = NULL;
    // msh_signals();
    // int status = 0;
    while(1)
    {
        cmd = msh_parse();
        // status = msh_execute(ast);
        free(cmd);
    }
}