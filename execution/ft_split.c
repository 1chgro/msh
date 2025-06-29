/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olachgue <olachgue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:40:37 by noel-baz          #+#    #+#             */
/*   Updated: 2025/06/29 15:13:59 by olachgue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	len_words(char const *str, char c)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = 0;
	while (str[i])
	{
		if (str[i] != c && (str[i + 1] == c || str[i + 1] == '\0'))
			len++;
		i++;
	}
	return (len);
}

static int	check_null(char **arr, int i)
{
	if (arr[i] == NULL)
	{
		while (i--)
			free(arr[i]);
		free(arr);
		return (0);
	}
	else
		return (1);
}

static char	*get_word(char const *str, char c)
{
	int		len;
	char	*word;

	len = 0;
	while (str[len] && str[len] != c)
		len++;
	word = malloc(len + 1);
	if (!word)
		return (NULL);
	word[len] = '\0';
	while (len--)
		word[len] = str[len];
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**split;
	size_t	i;

	if (s == NULL)
		return (NULL);
	i = 0;
	split = malloc((len_words(s, c) + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s && *s != c)
		{
			split[i] = get_word(s, c);
			if (!check_null(split, i))
				return (NULL);
			i++;
		}
		while (*s && *s != c)
			s++;
	}
	split[i] = NULL;
	return (split);
}
