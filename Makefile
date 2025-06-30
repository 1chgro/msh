NAME=minishell

FLGS= -Werror -Wextra -Wall

SRCS_PARSE= main.c\
	parsing/parse.c parsing/signals.c parsing/token.c parsing/quotes.c parsing/ft_utils.c parsing/ft_utils_2.c\
	parsing/is_funs.c parsing/token_utils.c parsing/syntax.c parsing/create_cmd.c\
	execution/copie_env.c execution/ft_split.c parsing/expanding.c parsing/expand_heredoc.c parsing/free_functions.c\
	parsing/export_expanding.c parsing/creat_cmd_lst.c parsing/fill_cmd.c parsing/quote2.c \
	parsing/create_cmd_utils.c parsing/expand_vars_utils.c parsing/global_expand.c parsing/expand_export_utils.c \
	parsing/expand_export_utils2.c

SRCS_EXEC=execution/ft_strcmp.c execution/exec.c execution/run_cmd.c  execution/builtins/ft_unset.c execution/builtins/ft_pwd.c\
		execution/builtins/ft_export.c execution/builtins/ft_exit.c execution/builtins/ft_env.c execution/builtins/ft_echo.c execution/builtins/ft_cd.c \
		execution/here_doc.c execution/run_cmd_utils.c execution/builtins/utils/cd_utils1.c \
		execution/builtins/utils/cd_utils2.c execution/builtins/utils/cd_utils3.c execution/builtins/utils/cd_utils4.c \
		execution/builtins/utils/cd_utils5.c execution/builtins/utils/exit_utils.c execution/builtins/utils/export_utils1.c execution/builtins/utils/export_utils2.c \
		execution/builtins/utils/export_utils3.c execution/exec_utils/run_cmd_utils_2.c execution/exec_utils/run_cmd_utils_3.c execution/exec_utils/run_cmd_utils_4.c \
		execution/exec_utils/run_cmd_utils_5.c execution/exec_utils/run_cmd_utils_6.c execution/exec_utils/run_cmd_utils_7.c execution/exec_utils/run_cmd_utils_8.c \
		execution/exec_utils/run_cmd_utils_9.c execution/exec_utils/exec_utils1.c execution/exec_utils/exec_utils2.c execution/copie_env_utils/copie_env_utils1.c \
		execution/copie_env_utils/copie_env_utils2.c 

OBJS=$(SRCS_PARSE:.c=.o) $(SRCS_EXEC:.c=.o)


READLINE_COMPILE = -I$(shell brew --prefix readline)/include
READLINE_LINK = -lreadline -L$(shell brew --prefix readline)/lib

all: $(NAME)

$(NAME): $(OBJS) minishell.h
	@cc $(FLGS) $(READLINE_COMPILE) $(OBJS) -o $(NAME) $(READLINE_LINK)
	@echo "Compilation done;"

%.o:%.c minishell.h
	@cc $(FLGS) $(READLINE_COMPILE) -c $< -o $@
clean:
	@rm -rf $(OBJS)
fclean: clean
	@rm -rf $(NAME)

re: fclean all