
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

static char *ft_cd_resolve_parent(char *old_pwd)
{
    char *resolved_path;
    char *last_slash;

    resolved_path = ft_strdup(old_pwd);
    if (!resolved_path)
        return (NULL);
    last_slash = strrchr(resolved_path, '/');
    if (last_slash && last_slash != resolved_path)
        *last_slash = '\0';
    else
    {
        free(resolved_path);
        resolved_path = ft_strdup("/");
        if (!resolved_path)
            return (NULL);
    }
    return (resolved_path);
}

void    ft_cd(char  **s_cmd, t_env  **env)
{
    char    *new_pwd;
    char    *path;
    char    *old_pwd;
    char    *temp;

    if (!s_cmd[1])
    {
        path = get_home(*env);
        printf("%s\n", path);
        if (!path)
        {
            perror("msh: cd:");
            return ;
        }
    }
    else if (s_cmd[1][0] == '~' && ft_strlen(s_cmd[1]) == 1)
    {
        path = get_home(*env);
        printf("%s\n", path);
        if (!path)
        {
            perror("msh: cd:");
            return ;
        }
    }
    else if (s_cmd[1][0] == '~' && ft_strlen(s_cmd[1]) > 1)
    {
        path = ft_strjoin_(get_home(*env), s_cmd[1] + 1);

        printf("%s\n", path);
        if (!path)
        {
            perror("msh: cd:");
            return ;
        }
    }
    else
    {
        if (!s_cmd[2])
            path = s_cmd[1];
        else
        {
            ft_putstr_fd("msh: cd: too many arguments\n", 2);
            return ;
        }
    }
    old_pwd = get_current_pwd();
    if (!old_pwd)
        return ;
    if (ft_strcmp(path, "..") == 0)
    {
        temp = ft_cd_resolve_parent(get_current_pwd());
        if (temp && access(temp, F_OK) == 0)
        {
            if (path != s_cmd[1])
                free(path);
            path = temp;
        }
        else
        {
            free(temp);
            perror("msh: cd: parent directory does not exist");
            free(old_pwd);
            return;
        }
    }
    if(chdir(path) == -1)
    {
        perror("msh: cd");
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
