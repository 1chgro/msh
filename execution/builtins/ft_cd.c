
#include "../../minishell.h"

char    *get_home(t_env *env)
{
    t_env   *temp = NULL;

    temp = env;
    while (temp)
    {
        if (ft_strcmp(temp->key, "HOME") == 0)
            return (temp->value);
        temp = temp->next;
    }
    return (NULL);
}

void set_env(t_env **env, char *key, char *value)
{
    t_env   *current = NULL;
    t_env   *new_node = NULL;

    current = *env;
    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
        {
            // free(current->value);
            current->value = ft_strdup(value);
            return;
        }
        current = current->next;
    }
    new_node = malloc(sizeof(t_env));
    if (!new_node)
        return;
    new_node->key = ft_strdup(key);
    new_node->value = ft_strdup(value);
    new_node->flag = 1;
    new_node->index = 0;
    new_node->next = NULL;
    append_node(env, new_node);
}


char *get_current_pwd(void)
{
    char *pwd = getcwd(NULL, 0);
    if (!pwd)
    {
        perror("minishell: getcwd");
        return NULL;
    }
    return pwd;
}

void    ft_cd(char  **s_cmd, t_env  **env)
{
    char    *new_pwd;
    char    *path;
    char    *old_pwd;
    char    **splited;

    if (!s_cmd[1] || s_cmd[1][0] == '~')
    {
        path = get_home(*env);
        printf("%s\n", path);
        if (!path)
        {
            perror("msh: cd: HOME not set\n");
            return ;
        }
    }
    else
    {
        splited = ft_split(s_cmd[1], ' ');
        path = splited[0];
    }
    old_pwd = get_current_pwd();
    if (!old_pwd)
        return ;
    if(chdir(path) == -1)
    {
        printf("msh: cd: %s: No such file or directory\n", path);
        free(old_pwd);
        return ;
    }
    new_pwd = get_current_pwd();
    if (!new_pwd)
    {
        free(old_pwd);
        return ;
    }
    set_env(env, "OLDPWD", old_pwd);
    set_env(env, "PWD", new_pwd);
    free(new_pwd);
    free(old_pwd);
}