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

t_ast *msh_parse(void)
{
    char *line;
    t_ast *ast;
    t_token *tokens;

    line = NULL;
    ast = NULL;
    line = read_line();
    if (line == NULL)
        return (NULL);
    tokens = lexer(line);
    t_token *tmp   = tokens;
    while (tmp)
    {
        printf("Token: %s\t|type: %u\n", tmp->value, tmp->type);
        tmp = tmp->next;
    }
    return (ast);
}

void msh_loop(void)
{
    t_ast *ast = NULL;
    // msh_signals();
    // int status = 0;
    while(1)
    {
        ast = msh_parse();
        // status = msh_execute(ast);
        free(ast);
    }
}