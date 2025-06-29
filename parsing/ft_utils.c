/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olachgue <olachgue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:48:53 by olachgue          #+#    #+#             */
/*   Updated: 2025/06/29 20:48:56 by olachgue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

char	*ft_strchr(const char *s, int c)
{
	char	*p;

	p = (char *)s;
	while (*p != '\0')
	{
		if (*p == (char )c)
			return (p);
		p++;
	}
	if ((char )c == '\0')
		return (p);
	return (NULL);
}

char	*ft_stringcpy(char *dst, const char *src, size_t n)
{
	size_t	i;

	if (!dst || !src)
		return (NULL);
	i = 0;
	while (i < n && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char	*ft_strtrim(char *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*trimmed_str;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	end = ft_strlen(s1);
	while (end > start && ft_strchr(set, s1[end - 1]))
		end--;
	trimmed_str = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!trimmed_str)
		return (free(s1), NULL);
	ft_stringcpy(trimmed_str, s1 + start, end - start);
	if (!trimmed_str)
		return (free(s1), NULL);
	return (free(s1), trimmed_str);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char		*jnd_str;
	size_t		total_len;
	size_t		i;
	size_t		j;

	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	(1) && (i = -1, j = 0);
	total_len = ft_strlen(s1) + ft_strlen(s2);
	jnd_str = malloc(sizeof(char) * (total_len + 2));
	if (!jnd_str)
		return (free((char *)s1), NULL);
	while (++i < ft_strlen(s1))
		jnd_str[i] = s1[i];
	jnd_str[i] = ' ';
	i++;
	while (s2[j] && i <= total_len)
		jnd_str[i++] = s2[j++];
	jnd_str[i] = '\0';
	free(s1);
	return (jnd_str);
}
