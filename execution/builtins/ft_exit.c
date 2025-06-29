/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noel-baz <noel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:49:00 by noel-baz          #+#    #+#             */
/*   Updated: 2025/06/29 20:49:01 by noel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	handle_sign(const char *str, int *i)
{
	int	sign;

	sign = 1;
	if (str[*i] == '-')
	{
		sign = -1;
		(*i)++;
	}
	else if (str[*i] == '+')
		(*i)++;
	return (sign);
}

static int	check_overflow(long result, char digit)
{
	if (result > LONG_MAX / 10)
		return (1);
	if (result == LONG_MAX / 10 && (digit - '0') > LONG_MAX % 10)
		return (1);
	return (0);
}

static long	parse_digits(const char *str, int *i, int *is_valid)
{
	long	result;

	result = 0;
	while (str[*i] >= '0' && str[*i] <= '9')
	{
		if (check_overflow(result, str[*i]))
		{
			*is_valid = 0;
			return (0);
		}
		result = result * 10 + (str[*i] - '0');
		(*i)++;
	}
	return (result);
}

long	ft_atoi_(const char *str, int *is_valid)
{
	int		i;
	long	result;
	int		sign;

	i = 0;
	*is_valid = 0;
	while (str[i] == ' ')
		i++;
	if (str[i] == '\0')
		return (0);
	sign = handle_sign(str, &i);
	result = parse_digits(str, &i, is_valid);
	if (*is_valid == 0)
		return (0);
	while (str[i] == ' ')
		i++;
	if (str[i] != '\0')
		return (0);
	*is_valid = 1;
	return (result * sign);
}

int	ft_exit(char **s_cmd, int last_ex)
{
	int	exit_num;
	int	is_valid;

	exit_num = 0;
	is_valid = 0;
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		write(STDERR_FILENO, "exit\n", 5);
	if (!s_cmd[1])
		exit(last_ex);
	exit_num = (int)ft_atoi_(s_cmd[1], &is_valid);
	handle_invalid_with_multiple_args(s_cmd, is_valid);
	if (handle_too_many_args_exit(s_cmd))
		return (1);
	if (!is_valid)
	{
		print_numeric_error(s_cmd[1]);
		exit(255);
	}
	exit_num = exit_num % 256;
	if (exit_num < 0)
		exit_num += 256;
	exit(exit_num);
}
