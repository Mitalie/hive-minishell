NAME = parser
CC = cc
RM = rm -f
CFLAGS = -g -Wall -Wextra -Werror -I include
# Source files by directory
AST_SRCS = src/ast/ast_command.c src/ast/ast_redirect.c src/ast/ast_pipeline.c \
           src/ast/ast_list.c src/ast/ast_free.c
PARSER_SRCS = src/parser/parser_command.c src/parser/parser_pipeline.c \
              src/parser/parser_list.c
UTILS_SRCS = src/utils/error.c src/utils/memory.c
# All source files
SRCS = $(AST_SRCS) $(PARSER_SRCS) $(UTILS_SRCS)
# Object files
OBJS = $(SRCS:.c=.o)
# Tests
TEST_DIR = test
TESTS = test_command test_redirect test_pipeline test_list test_integrated test_group
TEST_SRCS = $(addprefix $(TEST_DIR)/, $(addsuffix .c, $(TESTS)))
TEST_OBJS = $(TEST_SRCS:.c=.o)
# LibFT
LIBFT_DIR = libft
LIBFT_NAME = $(LIBFT_DIR)/libft.a
LIBFT_SRCS = $(wildcard $(LIBFT_DIR)/src/*.c)
LIBFT_OBJS = $(LIBFT_SRCS:$(LIBFT_DIR)/src/%.c=$(LIBFT_DIR)/obj/%.o)

all: $(LIBFT_NAME) $(NAME)

$(NAME): $(OBJS) $(LIBFT_NAME)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -o $(NAME)

# Test targets
test: $(addprefix $(TEST_DIR)/, $(TESTS))

$(TEST_DIR)/test_%: $(TEST_DIR)/test_%.o $(OBJS) $(LIBFT_NAME)
	$(CC) $(CFLAGS) $< $(OBJS) -L$(LIBFT_DIR) -lft -o $@

# LibFT compilation
$(LIBFT_NAME): $(LIBFT_OBJS)
	ar rcs $(LIBFT_NAME) $(LIBFT_OBJS)

$(LIBFT_DIR)/obj/%.o: $(LIBFT_DIR)/src/%.c
	mkdir -p $(LIBFT_DIR)/obj
	$(CC) $(CFLAGS) -c $< -o $@

# Compile each source file to its object file
%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Individual test targets for convenience
test_command: $(TEST_DIR)/test_command
test_redirect: $(TEST_DIR)/test_redirect
test_pipeline: $(TEST_DIR)/test_pipeline
test_list: $(TEST_DIR)/test_list
test_integrated: $(TEST_DIR)/test_integrated
test_group: $(TEST_DIR)/test_group

clean:
	$(RM) $(OBJS) $(TEST_OBJS) $(LIBFT_OBJS)

fclean: clean
	$(RM) $(NAME) $(LIBFT_NAME)
	$(RM) $(addprefix $(TEST_DIR)/, $(TESTS))

re: fclean all

.PHONY: all clean fclean re test test_command test_redirect test_pipeline test_list test_group
