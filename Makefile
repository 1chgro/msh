NAME=minishell

FLGS=-Werror -Wextra -Wall #-g -fsanitize=address


SRCS_PARSE= main.c\
	parsing/parse.c parsing/signals.c parsing/token.c parsing/quotes.c parsing/ft_utils.c parsing/ft_utils_2.c\
	parsing/is_funs.c parsing/token_utils.c parsing/syntax.c parsing/create_cmd.c\
	execution/copie_env.c execution/ft_split.c parsing/expanding.c parsing/expand_heredoc.c parsing/free_functions.c parsing/print_functions.c

SRCS_EXEC=execution/ft_strcmp.c execution/exec.c execution/run_cmd.c  execution/builtins/ft_unset.c execution/builtins/ft_pwd.c\
		execution/builtins/ft_export.c execution/builtins/ft_exit.c execution/builtins/ft_env.c execution/builtins/ft_echo.c execution/builtins/ft_cd.c \
		execution/here_doc.c execution/get_next_line.c execution/next_line_utils.c execution/run_cmd_utils.c 

OBJS=$(SRCS_PARSE:.c=.o) $(SRCS_EXEC:.c=.o)

INC=parse.h execution/exec.h


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