/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noel-baz <noel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:51:00 by noel-baz          #+#    #+#             */
/*   Updated: 2025/06/29 20:51:01 by noel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	get_num(int n)
{
	int	count;

	count = 0;
	if (n < 0)
		count++;
	while (n != 0)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char	*res;
	int		size;
	int		num;

	num = n;
	if (num == -2147483648)
		return (ft_strdup("-2147483648"));
	if (num == 0)
		return (ft_strdup("0"));
	size = get_num(num);
	res = malloc(sizeof(char) * (size + 1));
	if (!res)
		return (NULL);
	res[size--] = '\0';
	if (num < 0)
	{
		res[0] = '-';
		num = -num;
	}
	while (num != 0)
	{
		res[size--] = (num % 10) + 48;
		num /= 10;
	}
	return (res);
}

void	get_terminall(struct termios *termios)
{
	if (tcgetattr(STDIN_FILENO, termios) == -1)
		perror("tcgetattr: ");
}

void	set_terminall(struct termios *termios)
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, termios) == -1)
	{
		perror("tcsetattr: ");
	}
}

char	*ft_strrchr(const char *s, int c)
{
	int	i;
	int	start;

	i = 0;
	start = -1;
	while (s[i])
	{
		if (s[i] == (char)c)
			start = i;
		i++;
	}
	if (s[i] == (char)c)
		return ((char *)s + i);
	if (start == -1)
		return (NULL);
	return ((char *)s + start);
}
