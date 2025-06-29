/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd_utils_9.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noel-baz <noel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:50:41 by noel-baz          #+#    #+#             */
/*   Updated: 2025/06/29 20:50:42 by noel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	setup_pipes(t_cmd *cmd, int *p_fd, int prev_fd)
{
	if (cmd->next && pipe(p_fd) == -1)
	{
		if (cmd->next)
		{
			close(p_fd[0]);
			close(p_fd[1]);
		}
		if (prev_fd != -1)
			close(prev_fd);
		return (1);
	}
	return (0);
}
