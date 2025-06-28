#include "../../../minishell.h"

void	print_numeric_error(char *arg)
{
	dup2(2, 1);
	printf("msh: exit: %s: numeric argument required\n", arg);
	dup2(1, 2);
}

int	handle_invalid_with_multiple_args(char **s_cmd, int is_valid)
{
	if (s_cmd[2] && !is_valid)
	{
		print_numeric_error(s_cmd[1]);
		exit(255);
	}
	return (0);
}

int	handle_too_many_args_exit(char **s_cmd)
{
	if (s_cmd[2])
	{
		dup2(2, 1);
		printf("bash: exit: too many arguments\n");
		dup2(1, 2);
		return (1);
	}
	return (0);
}
