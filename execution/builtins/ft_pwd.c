#include "../../minishell.h"

int	ft_pwd(t_env *env)
{
	char	buff[8192000];
	char	*pwd;

	(void)env;
	if (!is_valid_cwd())
	{
		pwd = take_store_pwd(NULL);
		if (!pwd)
		{
			ft_putstr_fd("pwd: error retrieving current directory: getcwd: "
				"cannot access parent directories:"
				" No such file or directory\n", 2);
			return (1);
		}
		printf("%s\n", pwd);
		free(pwd);
		return (0);
	}
	getcwd(buff, 8192000);
	take_store_pwd(buff);
	printf("%s\n", buff);
	return (0);
}
