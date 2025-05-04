NAME=minishell

FLGS=-Werror -Wextra -Wall -fsanitize=address


SRCS_PARSE= main.c\
	parsing/parse.c parsing/signals.c parsing/token.c parsing/quotes.c parsing/ft_strtrim.c\
	parsing/ft_strdup.c parsing/is_funs.c parsing/token_utils.c parsing/syntax.c parsing/create_cmd.c


OBJS=$(SRCS_PARSE:.c=.o) $(SRCS_EXEC:.c=.o)

INC=parse.h execution/exec.h


READLINE_COMPILE = -I$(shell brew --prefix readline)/include
READLINE_LINK = -lreadline -L$(shell brew --prefix readline)/lib

all: $(NAME)

$(NAME): $(OBJS)
	cc $(FLGS) $(READLINE_COMPILE) $(OBJS) -o $(NAME) $(READLINE_LINK)

%.o:%.c
	cc $(FLGS) $(READLINE_COMPILE) -c $< -o $@
clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all
