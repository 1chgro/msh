/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olachgue <olachgue@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:09:50 by olachgue          #+#    #+#             */
/*   Updated: 2025/06/29 14:09:51 by olachgue         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **envp)
{
	(void)av;
	if (ac > 1)
		return (write(2, "Usage: ./minishell\n", 20), 1);
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		msh_loop(envp);
	return (0);
}
