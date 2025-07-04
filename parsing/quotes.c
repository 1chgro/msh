/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olachgue <olachgue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:49:32 by olachgue          #+#    #+#             */
/*   Updated: 2025/06/29 20:49:38 by olachgue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	valid_quotes(char *s)
{
	int		i;
	char	q;

	i = 0;
	while (s[i])
	{
		if (is_quote(s[i]))
		{
			q = s[i];
			i++;
			while (s[i] && s[i] != q)
				i++;
			if (s[i] == '\0')
				return (0);
		}
		i++;
	}
	return (1);
}

void	skip_quotes(char *s, int *i, char q)
{
	(*i)++;
	while (s[*i] && s[*i] != q)
		(*i)++;
	if (s[*i] != '\0')
		(*i)++;
}

static int	should_skip_quote(char c, char *quote)
{
	if (!*quote && (c == '"' || c == '\''))
	{
		*quote = c;
		return (1);
	}
	else if (*quote && c == *quote)
	{
		*quote = 0;
		return (1);
	}
	return (0);
}

char	*remove_outer_quotes(char *s)
{
	int		i;
	int		j;
	char	quote;
	char	*output;
	int		len;

	if (!s)
		return (NULL);
	(1) && (i = 0, j = 0, quote = 0, output = NULL, len = ft_strlen(s));
	output = malloc(len + 1);
	if (!output)
		return (free(s), NULL);
	while (s[i])
	{
		if (should_skip_quote(s[i], &quote))
		{
			i++;
			continue ;
		}
		if ((quote == '"' && s[i] == '\'') || (quote == '\'' && s[i] == '"'))
			output[j++] = s[i++];
		else
			output[j++] = s[i++];
	}
	return (output[j] = '\0', output);
}

char	**remove_quotes_arr(char **argv)
{
	int		i;
	int		j;
	char	**new_argv;
	int		len;

	if (!argv)
		return (NULL);
	(1) && (i = 0, j = 0, new_argv = NULL, len = 0);
	while (argv[len])
		len++;
	new_argv = malloc(sizeof(char *) * (len + 1));
	if (!new_argv)
		return (free_arr(argv), NULL);
	while (argv[i])
	{
		new_argv[i] = remove_outer_quotes(argv[i]);
		if (!new_argv)
		{
			free_arr(argv);
			free_arr(new_argv);
			return (NULL);
		}
		i++;
	}
	return (new_argv[i] = NULL, free_arr(argv), new_argv);
}
