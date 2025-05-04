#include "parse.h"


int main(int ac, char **av)
{
    (void)av;
    if (ac > 1)
        return (write(2, "Usage: ./minishell\n", 20), 1);
    msh_loop();
    return (0);
}