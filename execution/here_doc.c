#include "../minishell.h"

int g_in_heredoc = 0;

void handel_sigint(int sig)
{
	if (sig == SIGINT)
	{
		g_in_heredoc = 1;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int here_doc(char *limiter, int *fd, int *expnd_flg, t_glob_st *glob_strct)
{
    char *line1;
    char *line;
    int write_fd;
    unlink("/tmp/file_tmp");
    write_fd = open("/tmp/file_tmp", O_CREAT | O_RDWR, 0644);
    *fd = open("/tmp/file_tmp", O_RDONLY);
    if (write_fd < 0 || *fd < 0)
    {
        perror("open failed");
        return (1);
    }
    unlink("/tmp/file_tmp");
    signal(SIGINT, handel_sigint);
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
            break;
        }
        if (*expnd_flg)
            line1 = expand_heredoc(line, glob_strct);
        else
            line1 = ft_strdup(line);
        free(line);
        write(write_fd, line1, ft_strlen(line1));
        write(write_fd, "\n", 1);
        free(line1);
    }
    msh_signals(glob_strct);
    if (g_in_heredoc)
    {
        g_in_heredoc = 0;
        close(write_fd);
        return (1);
    }
    close(write_fd);
    return (0);
}
