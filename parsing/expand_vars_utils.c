
#include "../minishell.h"

int	check_for_ambgu(char *name)
{
	int	i;

	if (!name || !*name)
		return (1);
	i = 0;
	while (name[i])
	{
		if (is_space(name[i]))
			return (1);
		i++;
	}
	return (0);
}

char	*space_change(char *str)
{
	char	*new_str;
	int		i;
	char	tmp[2];

	if (!str)
		return (NULL);
	i = 0;
	tmp[0] = '\0';
	tmp[1] = '\0';
	new_str = NULL;
	while (str[i])
	{
		if (is_space(str[i]))
			tmp[0] = ' ';
		else
			tmp[0] = str[i];
		tmp[1] = '\0';
		new_str = ft_strjoin_ws(new_str, tmp);
		if (!new_str)
			return (free(str), NULL);
		i++;
	}
	return (free(str), new_str);
}

char	*handle_exit_status_expansion(int *i, \
		t_glob_st *glob_strct, char *result)
{
	char	*var_value;

	var_value = ft_itoa(glob_strct->ext_stat);
	result = ft_strjoin_ws(result, var_value);
	(*i)++;
	free(var_value);
	return (result);
}

char	*add_quotes(char *value)
{
	char	*result;

	if (!value)
		return (NULL);
	result = NULL;
	if (value[0] == '"' || value[0] == '\'')
		result = ft_strdup(value);
	else
	{
		result = malloc(sizeof(char) * (ft_strlen(value) + 3));
		if (!result)
			return (NULL);
		result[0] = '"';
		ft_stringcpy(&result[1], value, ft_strlen(value));
		result[ft_strlen(value) + 1] = '"';
		result[ft_strlen(value) + 2] = '\0';
	}
	free(value);
	return (result);
}
