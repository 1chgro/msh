
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

 int is_valid_cwd(void)
 {
    char *cwd = getcwd(NULL, 0);
    if (!cwd)
    {
        return 0;
    }
    free(cwd);
    return 1;
}

static char *ft_cd_resolve_parent(char *old_pwd)
{
    char *resolved_path = ft_strdup(old_pwd);
    if (!resolved_path)
    {
        return NULL;
    }
    char *last_slash = strrchr(resolved_path, '/');
    if (last_slash && last_slash != resolved_path)
    {
        *last_slash = '\0';
    }
    else
    {
        free(resolved_path);
        resolved_path = ft_strdup("/");
        if (!resolved_path)
        {
            return NULL;
        }
    }
    if (access(resolved_path, F_OK) != 0)
    {
        free(resolved_path);
        return NULL;
    }
    return resolved_path;
}
int ft_cd(char **s_cmd, t_env **env)
{
    char *new_pwd = NULL;
    char *path = NULL;
    char *old_pwd = NULL;
    char *temp = NULL;

    old_pwd = my_getenv("PWD", *env);
    if (!old_pwd)
    {
        old_pwd = get_current_pwd();
        if (!old_pwd)
        {
            old_pwd = ft_strdup("/");
            if (!old_pwd)
            {
                ft_putstr_fd("msh: cd: cannot allocate memory\n", 2);
                return 1;
            }
        }
    }
    else
    {
        old_pwd = ft_strdup(old_pwd);
        if (!old_pwd)
        {
            ft_putstr_fd("msh: cd: cannot allocate memory\n", 2);
            return 1;
        }
    }
    if (!s_cmd[1])
    {
        path = get_home(*env);
        if (!path)
        {
            ft_putstr_fd("msh: cd: HOME not set\n", 2);
            free(old_pwd);
            return 1;
        }
    }
    else if (s_cmd[1][0] == '~' && strlen(s_cmd[1]) == 1)
    {
        path = get_home(*env);
        if (!path)
        {
            ft_putstr_fd("msh: cd: HOME not set\n", 2);
            free(old_pwd);
            return 1;
        }
    }
    else if (s_cmd[1][0] == '-' && strlen(s_cmd[1]) == 1)
    {
        path = my_getenv("OLDPWD", *env);
        if (!path)
        {
            ft_putstr_fd("msh: cd: OLDPWD not set\n", 2);
            free(old_pwd);
            return 1;
        }
    }
    else if (s_cmd[1][0] == '~' && strlen(s_cmd[1]) > 1)
    {
        char *home = get_home(*env);
        if (!home)
        {
            ft_putstr_fd("msh: cd: HOME not set\n", 2);
            free(old_pwd);
            return 1;
        }
        path = ft_strjoin_(home, s_cmd[1] + 1);
        if (!path)
        {
            ft_putstr_fd("msh: cd: cannot allocate memory\n", 2);
            free(old_pwd);
            return 1;
        }
    }
    else
    {
        if (s_cmd[2])
        {
            ft_putstr_fd("msh: cd: too many arguments\n", 2);
            free(old_pwd);
            return 1;
        }
        path = s_cmd[1];
    }

    if (ft_strcmp(path, "..") == 0)
    {
        temp = ft_cd_resolve_parent(old_pwd);
        if (!temp)
        {
            char *last_slash = strrchr(old_pwd, '/');
            if (last_slash && last_slash != old_pwd)
            {
                temp = ft_strndup(old_pwd, last_slash - old_pwd);
            }
            else
            {
                temp = ft_strdup("/");
            }
            if (!temp)
            {
                ft_putstr_fd("msh: cd: cannot allocate memory\n", 2);
                free(old_pwd);
                return 1;
            }
        }
        path = temp;
    }
    printf("%s\n", path);
    if (chdir(path) == -1)
    {
        if (ft_strcmp(s_cmd[1], "..") == 0)
        {
            set_env(env, "PWD", path);
        }
        perror("msh: cd");
        if (path == temp) free(temp);
        free(old_pwd);
        return 1;
    }
    new_pwd = get_current_pwd();
    if (!new_pwd)
    {
        ft_putstr_fd("msh: cd: cannot get current directory\n", 2);
        if (path == temp)
            free(temp);
        free(old_pwd);
        return 1;
    }
    if (old_pwd)
    {
        set_env(env, "OLDPWD", old_pwd);
    }
    set_env(env, "PWD", new_pwd);
    if (path == temp)
        free(temp);
    free(new_pwd);
    free(old_pwd);
    return 0;
}