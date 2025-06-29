/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noel-baz <noel-baz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:50:57 by noel-baz          #+#    #+#             */
/*   Updated: 2025/06/29 20:50:58 by noel-baz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handel_sigint(int sig)
{
	if (sig == SIGINT)
	{
		g_in_heredoc = 1;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	setup_heredoc_files(int *fd, int *write_fd)
{
	unlink("/tmp/file_tmp");
	*write_fd = open("/tmp/file_tmp", O_CREAT | O_RDWR, 0644);
	*fd = open("/tmp/file_tmp", O_RDONLY);
	if (*write_fd < 0 || *fd < 0)
	{
		perror("open failed");
		return (1);
	}
	unlink("/tmp/file_tmp");
	return (0);
}

void	process_heredoc_line(char *line, int write_fd, \
		int *expnd_flg, t_glob_st *glob_strct)
{
	char	*line1;

	if (*expnd_flg)
		line1 = expand_heredoc(line, glob_strct);
	else
		line1 = ft_strdup(line);
	free(line);
	write(write_fd, line1, ft_strlen(line1));
	write(write_fd, "\n", 1);
	free(line1);
}

int	read_heredoc_input(char *limiter, int write_fd, \
		int *expnd_flg, t_glob_st *glob_strct)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || g_in_heredoc)
		{
			if (g_in_heredoc == 1)
				glob_strct->ext_stat = 1;
			free(line);
			break ;
		}
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			break ;
		}
		process_heredoc_line(line, write_fd, expnd_flg, glob_strct);
	}
	return (0);
}

int	here_doc(char *limiter, int *fd, int *expnd_flg, t_glob_st *glob_strct)
{
	int	write_fd;

	if (setup_heredoc_files(fd, &write_fd) != 0)
		return (1);
	signal(SIGINT, handel_sigint);
	read_heredoc_input(limiter, write_fd, expnd_flg, glob_strct);
	msh_signals();
	if (g_in_heredoc)
	{
		g_in_heredoc = 0;
		close(write_fd);
		return (1);
	}
	close(write_fd);
	return (0);
}
