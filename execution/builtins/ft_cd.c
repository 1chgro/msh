#include "../../minishell.h"

char	*get_home(t_env *env)
{
	t_env	*temp;

	temp = env;
	while (temp)
	{
		if (ft_strcmp(temp->key, "HOME") == 0)
			return (temp->value);
		temp = temp->next;
	}
	return (NULL);
}

static t_env	*find_env_key(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

static t_env	*create_new_env_node(char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->flag = 1;
	new_node->index = 0;
	new_node->next = NULL;
	return (new_node);
}

void	set_env(t_env **env, char *key, char *value)
{
	t_env	*current;
	t_env	*new_node;

	current = find_env_key(*env, key);
	if (current)
	{
		current->value = ft_strdup(value);
		current->flag = 1;
		return ;
	}
	new_node = create_new_env_node(key, value);
	if (!new_node)
		return ;
	append_node(env, new_node);
}

char	*get_current_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (NULL);
	return (pwd);
}

int	is_valid_cwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (0);
	free(cwd);
	return (1);
}

char	*take_store_pwd(char *path)
{
	static char	*stored_pwd = NULL;

	if (path)
	{
		if (stored_pwd)
			free(stored_pwd);
		stored_pwd = ft_strdup(path);
		return (stored_pwd);
	}
	if (stored_pwd)
		return (ft_strdup(stored_pwd));
	return (NULL);
}

int	all(char *path)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (path[i])
	{
		if (path[i] == '.' && path[i + 1] == '.' && path[i + 2] != '.')
		{
			len++;
			i = i + 2;
		}
		else
			return (-1);
		if (path[i] && path[i] == '/')
			i++;
	}
	return (len);
}

static char	*get_old_pwd_or_default()
{
	char	*old_pwd;

	old_pwd = get_current_pwd();
	if (!old_pwd)
	{
		old_pwd = take_store_pwd(NULL);
		if (!old_pwd)
			old_pwd = ft_strdup("/");
		if (!old_pwd)
		{
			ft_putstr_fd("msh: cd: cannot allocate memory\n", 2);
			return (NULL);
		}
	}
	else
	{
		old_pwd = ft_strdup(old_pwd);
		if (!old_pwd)
			ft_putstr_fd("msh: cd: cannot allocate memory\n", 2);
	}
	return (old_pwd);
}

static char	*handle_no_args(t_env *env, char *old_pwd)
{
	char	*path;

	path = get_home(env);
	if (!path)
	{
		ft_putstr_fd("msh: cd: HOME not set\n", 2);
		free(old_pwd);
		return (NULL);
	}
	if (path[0] == '\0')
		path = ft_strdup(".");
	return (path);
}

static char	*handle_tilde_only(t_env *env, char *old_pwd)
{
	char	*path;

	path = get_home(env);
	if (!path)
	{
		ft_putstr_fd("msh: cd: HOME not set\n", 2);
		free(old_pwd);
		return (NULL);
	}
	if (path[0] == '\0')
		path = ft_strdup(".");
	return (path);
}

static char	*handle_dash_only(t_env *env, char *old_pwd)
{
	char	*path;

	path = my_getenv("OLDPWD", env);
	if (!path)
	{
		ft_putstr_fd("msh: cd: OLDPWD not set\n", 2);
		free(old_pwd);
		return (NULL);
	}
	else if (path && path[0] != '\0')
		printf("%s\n", path);
	return (path);
}

static char	*handle_tilde_path(t_env *env, char *arg, char *old_pwd)
{
	char	*home;
	char	*path;

	home = get_home(env);
	if (!home)
	{
		ft_putstr_fd("msh: cd: HOME not set\n", 2);
		free(old_pwd);
		return (NULL);
	}
	path = ft_strjoin(home, arg + 1);
	if (!path)
	{
		ft_putstr_fd("msh: cd: cannot allocate memory\n", 2);
		free(old_pwd);
		return (NULL);
	}
	return (path);
}

static char	*determine_path(char **s_cmd, t_env *env, char *old_pwd)
{
	if (!s_cmd[1])
		return (handle_no_args(env, old_pwd));
	else if (s_cmd[1][0] == '~' && ft_strlen(s_cmd[1]) == 1)
		return (handle_tilde_only(env, old_pwd));
	else if (s_cmd[1][0] == '-' && ft_strlen(s_cmd[1]) == 1)
		return (handle_dash_only(env, old_pwd));
	else if (s_cmd[1][0] == '~' && ft_strlen(s_cmd[1]) > 1)
		return (handle_tilde_path(env, s_cmd[1], old_pwd));
	else
		return (s_cmd[1]);
}

static char	*create_logical_dotdot_path(char *old_pwd)
{
	char	*slashdotdot;

	if (old_pwd[ft_strlen(old_pwd) - 1] == '/')
		slashdotdot = ft_strjoin_(old_pwd, "..");
	else
		slashdotdot = ft_strjoin_(old_pwd, "/..");
	if (!slashdotdot)
		ft_putstr_fd("msh: cd: cannot allocate memory\n", 2);
	return (slashdotdot);
}

static int	handle_dotdot_logic(char *path, char *old_pwd, char **logical_pwd)
{
	if (ft_strcmp(path, "..") == 0 && access(path, X_OK) == 0)
	{
		*logical_pwd = create_logical_dotdot_path(old_pwd);
		if (!*logical_pwd)
			return (1);
		return (2);
	}
	return (0);
}

static char	*navigate_up_directories(char *old_pwd, int nbr)
{
	char	*temp_path;
	char	*to_rm;
	char	*new_path;

	temp_path = ft_strdup(old_pwd);
	if (!temp_path)
		return (NULL);
	while (nbr > 0)
	{
		to_rm = strrchr(temp_path, '/');
		if (to_rm == temp_path)
		{
			temp_path = ft_strdup("/");
			break ;
		}
		new_path = ft_strndup(temp_path, to_rm - temp_path);
		if (!new_path)
			return (NULL);
		temp_path = new_path;
		nbr--;
	}
	return (temp_path);
}

static int	handle_dotdot_chdir_fail(char *path, char *old_pwd)
{
	int		nbr;
	char	*temp_path;

	nbr = all(path);
	if (ft_strcmp(path, "..") != 0 && nbr <= 0)
		return (1);
	if (nbr < 0)
		nbr = 1;
	temp_path = navigate_up_directories(old_pwd, nbr);
	if (!temp_path)
	{
		ft_putstr_fd("msh: cd: cannot allocate memory\n", 2);
		return (1);
	}
	if (chdir(temp_path) == -1)
	{
		perror("msh: cd");
		return (1);
	}
	return (0);
}

static int	attempt_chdir(char *path, char *old_pwd)
{
	if (chdir(path) == -1)
	{
		if (handle_dotdot_chdir_fail(path, old_pwd) != 0)
		{
			perror("msh: cd");
			return (1);
		}
	}
	return (0);
}

static void	update_pwd_env(t_env **env, char *logical_pwd, char *path,
		int logical_dotdot)
{
	char	*new_pwd;

	new_pwd = get_current_pwd();
	if (!new_pwd)
	{
		ft_putstr_fd("cd: error retrieving current directory: getcwd: "
			"cannot access parent directories: No such file or directory\n", 2);
		if (logical_dotdot)
		{
			set_env(env, "PWD", logical_pwd);
			take_store_pwd(logical_pwd);
		}
		else
		{
			set_env(env, "PWD", path);
			if (ft_strcmp(path, "."))
				take_store_pwd(path);
		}
	}
	else
	{
		set_env(env, "PWD", new_pwd);
		take_store_pwd(new_pwd);
		free(new_pwd);
	}
}

int	ft_cd(char **s_cmd, t_env **env)
{
	char	*path;
	char	*old_pwd;
	char	*logical_pwd;
	int		logical_dotdot;

	logical_pwd = NULL;
	if (!env || !*env)
		return (1);
	old_pwd = get_old_pwd_or_default();
	if (!old_pwd)
		return (1);
	path = determine_path(s_cmd, *env, old_pwd);
	if (!path)
		return (1);
	logical_dotdot = handle_dotdot_logic(path, old_pwd, &logical_pwd);
	if (logical_dotdot == 1)
		return (1);
	if (logical_dotdot == 2)
		logical_dotdot = 1;
	else
		logical_dotdot = 0;
	if (attempt_chdir(path, old_pwd) != 0)
		return (1);
	set_env(env, "OLDPWD", old_pwd);
	update_pwd_env(env, logical_pwd, path, logical_dotdot);
	if (logical_pwd)
		free(logical_pwd);
	free(old_pwd);
	return (0);
}
