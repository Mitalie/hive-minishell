NAME = parser

CC = gcc
RM = rm -f
CFLAGS = -g -Wall -Wextra -Werror -I include -I libft/include

SRCS = src/ast.c src/parser.c src/utils.c src/test_parser.c
OBJS = $(SRCS:.c=.o)

LIBFT_DIR = libft
LIBFT_NAME = $(LIBFT_DIR)/libft.a
LIBFT_SRCS = $(wildcard $(LIBFT_DIR)/src/*.c)
LIBFT_OBJS = $(LIBFT_SRCS:$(LIBFT_DIR)/src/%.c=$(LIBFT_DIR)/obj/%.o)

all: $(LIBFT_NAME) $(NAME)

$(NAME): $(OBJS) $(LIBFT_NAME)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -o $(NAME)

$(LIBFT_NAME): $(LIBFT_OBJS)
	ar rcs $(LIBFT_NAME) $(LIBFT_OBJS)

$(LIBFT_DIR)/obj/%.o: $(LIBFT_DIR)/src/%.c
	mkdir -p $(LIBFT_DIR)/obj
	$(CC) $(CFLAGS) -c $< -o $@

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(LIBFT_OBJS)

fclean: clean
	$(RM) $(NAME) $(LIBFT_NAME)

re: fclean all

.PHONY: all clean fclean re
