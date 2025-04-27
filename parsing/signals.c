#include "../parse.h"

void handle_signals(int sig)
{
    if (sig == SIGINT)
    {
        write(1, "\n", 1);
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }
}


void msh_signals(void)
{
    signal(SIGINT, handle_signals);
    signal(SIGQUIT, SIG_IGN);
    rl_catch_signals = 0;
}