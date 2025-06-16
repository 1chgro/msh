#include "../../minishell.h"

static int is_valid_cwd(void)
{
    char *cwd;

    cwd = getcwd(NULL, 0);
    if (!cwd) {
        return 0;
    }
    free(cwd);
    return 1;
}

int ft_pwd(t_env *env)
{
    char buff[8192000];
    char *pwd;

    if (!is_valid_cwd())
    {
        pwd = my_getenv("PWD", env);
        if (!pwd)
            pwd = take_store_pwd(NULL); 
        if (!pwd)
            pwd = "/";
        printf("%s\n", pwd);
        return 0;
    }
    getcwd(buff, 8192000);
    take_store_pwd(buff);
    printf("%s\n", buff);
    return 0;
}
