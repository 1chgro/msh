
#include "../minishell.h"

char	*expand_key(char *key_str, t_glob_st *glob_strct)
{
	char	*key;

	key = expand(key_str, glob_strct);
	if (!key)
		return (NULL);
	return (key);
}

char	*process_value_expansion(char *value_str, \
		t_glob_st *glob_strct, int split_value)
{
	char	*value;
	char	*temp;
	char	**value_arr;
	int		k;

	(1) && (k = 0, value_arr = NULL, value = expand(value_str, glob_strct));
	if (!split_value)
	{
		temp = value;
		value = remove_outer_quotes(temp);
		free(temp);
		value = add_quotes(value);
	}
	else
	{
		value_arr = split_line_to_args(value);
		free(value);
		if (!value_arr)
			return (NULL);
		value = NULL;
		while (value_arr && value_arr[k])
			value = ft_strjoin(value, value_arr[k++]);
	}
	return (free_arr(value_arr), value);
}

int	should_split_value(int split_all_values, char *key_str, char *value_str)
{
	if (split_all_values || (check_key(key_str) == 1 \
		&& check_value(value_str) == 1))
		return (1);
	return (0);
}
