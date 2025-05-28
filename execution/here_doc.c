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
int here_doc(char *limiter, int *fd, t_glob_st *glob_strct)
{
    char *line1;
    char *line;
    int write_fd;
    write_fd = open("/tmp/file_tmp", O_CREAT | O_RDWR, 0644);
    *fd = open("/tmp/file_tmp", O_RDONLY);
    if (write_fd < 0 || *fd < 0)
    {
        perror("open failed");
        return (1);
    }
    unlink("/tmp/file_tmp");
    write(STDOUT_FILENO, "> ", 2);
    signal(SIGINT, handel_sigint);
    while (1)
    {
        line = readline(NULL);
        if (!line || g_in_heredoc)
        {
            if (g_in_heredoc)
            {
                glob_strct->ext_stat = 130;
                write(STDOUT_FILENO, "\n", 1);
            }
            free(line);
            break ;
        }
        if (ft_strcmp(line, limiter) == 0)
        {
            free(line);
            break;
        }
        line1 = expand_heredoc(line, glob_strct);
        free(line);
        write(write_fd, line1, ft_strlen(line1));
        write(write_fd, "\n", 1);
        free(line1);
        write(STDOUT_FILENO, "> ", 2);
    }
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
