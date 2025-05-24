#include "../minishell.h"

int here_doc(char *limiter, int *fd, t_env *env)
{
    char    *line1;
    char    *line;
    int     write_fd;

    write_fd = open("/tmp/file_tmp", O_CREAT | O_RDWR, 0644);
    *fd = open("/tmp/file_tmp", O_RDONLY);
    if (write_fd < 0 || *fd < 0)
    {
        perror("open failed");
        return (1);
    }
    unlink("/tmp/file_tmp");
    write(STDOUT_FILENO, "> ", 2);
    while (1)
    {
        line = readline(NULL);
        if (!line)
        {
            perror("readline failed");
            close(write_fd);
            return (1);
        }
        if (ft_strcmp(line, limiter) == 0)
        {
            free(line);
            break;
        }
        line1 = expand_heredoc(line, env);
        free(line);
        write(write_fd, line1, ft_strlen(line1));
        write(write_fd, "\n", 1);
        free(line1);
        write(STDOUT_FILENO, "> ", 2);
    }
    close(write_fd);
    return (0);
}
