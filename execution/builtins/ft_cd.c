
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
        // perror("minishell: getcwd");
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
char *take_store_pwd(char *path)
{
    static char *stored_pwd = NULL;

    if (path)
    {
        if (stored_pwd)
            free(stored_pwd);
        stored_pwd = ft_strdup(path);
        return stored_pwd;
    }
    if (stored_pwd)
        return ft_strdup(stored_pwd);
    return NULL;
}

int ft_cd(char **s_cmd, t_env **env)
{
    char *new_pwd = NULL;
    char *path = NULL;
    char *old_pwd = NULL;
    char *logical_pwd = NULL;

    if (!env || !*env)
        return (1);
    old_pwd = get_current_pwd();
    if (!old_pwd)
    {
        old_pwd = take_store_pwd(NULL);
        if (!old_pwd)
            old_pwd = ft_strdup("/");
        if (!old_pwd)
        {
            ft_putstr_fd("msh: cd: cannot allocate memory\n", 2);
            return 1;
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
    else if (s_cmd[1][0] == '~' && ft_strlen(s_cmd[1]) == 1)
    {
        path = get_home(*env);
        if (!path)
        {
            ft_putstr_fd("msh: cd: HOME not set\n", 2);
            free(old_pwd);
            return 1;
        }
    }
    else if (s_cmd[1][0] == '-' && ft_strlen(s_cmd[1]) == 1)
    {
        path = my_getenv("OLDPWD", *env);
        if (!path)
        {
            ft_putstr_fd("msh: cd: OLDPWD not set\n", 2);
            free(old_pwd);
            return 1;
        }
    }
    else if (s_cmd[1][0] == '~' && ft_strlen(s_cmd[1]) > 1)
    {
        char *home = get_home(*env);
        if (!home)
        {
            ft_putstr_fd("msh: cd: HOME not set\n", 2);
            free(old_pwd);
            return 1;
        }
        path = ft_strjoin(home, s_cmd[1] + 1);
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
    int logical_dotdot = 0;
    if (ft_strcmp(path, "..") == 0)
    {
        if (access(path, X_OK) != 0)
        {
            char *to_rm = strrchr(old_pwd, '/');
            path = ft_strndup(old_pwd, ft_strlen(old_pwd)-ft_strlen(to_rm));
        }
        else
        {
            logical_dotdot = 1;
            char *slashdotdot = NULL;
            if (old_pwd[ft_strlen(old_pwd) - 1] == '/')
                slashdotdot = ft_strjoin_(old_pwd, "..");
            else
                slashdotdot = ft_strjoin_(old_pwd, "/..");
            if (!slashdotdot)
            {
                ft_putstr_fd("msh: cd: cannot allocate memory\n", 2);
                free(old_pwd);
                return 1;
            }
            logical_pwd = slashdotdot;
            path = "..";
        }
    }
    printf("%s\n", path);
    if (chdir(path) == -1)
    {
        perror("msh: cd");
        if (logical_pwd)
            free(logical_pwd);
        free(old_pwd);
        return 1;
    }
    set_env(env, "OLDPWD", old_pwd);
    new_pwd = get_current_pwd();
    if (!new_pwd)
    {
        ft_putstr_fd("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n", 2);
        if (logical_dotdot){
            set_env(env, "PWD", logical_pwd);
            take_store_pwd(logical_pwd);
            }
        else
            {set_env(env, "PWD", path);
            take_store_pwd(path);
            }
    }
    else
    {
        set_env(env, "PWD", new_pwd);
        take_store_pwd(new_pwd);
        free(new_pwd);
    }
    if (logical_pwd)
        free(logical_pwd);
    free(old_pwd);
    return 0;
}