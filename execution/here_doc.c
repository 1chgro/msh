#include "../minishell.h"

void here_doc(char *limiter, int *fd)
{
    char    *line;
    int     write_fd;
    write_fd = open("/tmp/file_tmp", O_CREAT | O_RDWR, 0644);
    *fd = open("/tmp/file_tmp", O_RDONLY);
    if (write_fd < 0 || *fd < 0)
    {
        perror("open failed");
        return;
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
            return;
        }
        if (ft_strcmp(line, limiter) == 0)
        {
            free(line);
            break;
        }
        write(write_fd, line, ft_strlen(line));
        write(write_fd, "\n", 1);
        free(line);
        write(STDOUT_FILENO, "> ", 2);
    }
    close(write_fd);
}
