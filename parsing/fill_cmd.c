/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olachgue <olachgue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:48:31 by olachgue          #+#    #+#             */
/*   Updated: 2025/06/30 19:52:01 by olachgue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	skip_argument(const char *str, int i)
{
	int		in_quotes;
	char	quote_char;

	if (!str)
		return (i);
	in_quotes = 0;
	quote_char = '\0';
	while (str[i])
	{
		if (is_quote(str[i]))
		{
			if (!in_quotes)
			{
				quote_char = str[i];
				in_quotes = 1;
			}
			else if (str[i] == quote_char)
				in_quotes = 0;
		}
		else if (is_space(str[i]) && !in_quotes)
			break ;
		i++;
	}
	return (i);
}

int	count_args(char *str)
{
	int		count;
	int		i;

	if (!str)
		return (1);
	i = 0;
	count = 0;
	while (str[i])
	{
		if (is_space(str[i]))
		{
			i++;
			continue ;
		}
		count++;
		i = skip_argument(str, i);
	}
	return (count);
}

static char	*extract_arg(const char *str, int *i)
{
	int		start;
	int		in_quotes;
	char	quote_char;
	char	*result;

	(1) && (start = *i, in_quotes = 0, quote_char = '\0', result = NULL);
	while (str[*i])
	{
		if (is_quote(str[*i]))
		{
			if (!in_quotes)
			{
				quote_char = str[*i];
				in_quotes = 1;
			}
			else if (str[*i] == quote_char)
				in_quotes = 0;
		}
		else if (is_space(str[*i]) && !in_quotes)
			break ;
		(*i)++;
	}
	result = ft_strndup(str + start, *i - start);
	return (result);
}

static char	*process_single_arg(char *line, int *i)
{
	char	*arg;

	while (is_space(line[*i]))
		(*i)++;
	arg = extract_arg(line, i);
	arg = replace_value_quotes(arg);
	if (is_space(line[*i]))
		(*i)++;
	return (arg);
}

char	**split_line_to_args(char *line)
{
	int		arg_count;
	char	**args;
	int		i;
	int		j;

	if (!line)
		return (NULL);
	i = 0;
	arg_count = count_args(line);
	args = malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
		return (NULL);
	j = 0;
	while (line && j < arg_count)
	{
		printf("line: %s\n", line);
		args[j] = process_single_arg(line, &i);
		if (!args[j])
			return (free_arr(args), NULL);
		j++;
	}
	args[arg_count] = NULL;
	return (args);
}
