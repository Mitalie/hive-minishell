# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: josmanov <josmanov@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/11 15:47:17 by amakinen          #+#    #+#              #
#    Updated: 2025/04/13 13:53:06 by josmanov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell

# Directories
OBJDIR := obj
SRCDIR := src
INCDIRS := include

# Project files and targets
SRCS := $(addprefix $(SRCDIR)/,\
	main.c \
	tokenizer.c \
	ast/ast_free.c \
	parser/parser.c \
	parser/parser_syntax_error.c \
	parser/parser_list.c \
	parser/parser_group.c \
	parser/parser_pipeline.c \
	parser/parser_simple_command.c \
	parser/parser_redirect.c \
	parser/parser_word.c \
	execute/simple_command.c \
	execute/pipeline.c \
	execute/list.c \
	env/init.c \
	env/get_set.c \
	env/utils.c \
)

OBJS := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
BINS := $(NAME)
$(NAME): $(OBJS) libft/libft.a
$(NAME): tgt_LDLIBS := -lreadline

# Tests
TESTDIR := test
TESTS := $(addprefix $(TESTDIR)/,\
	tokenizer \
	input_parser \
	simple_command \
	pipeline \
	list \
	env \
)
TEST_SRCS := $(TESTS:%=$(SRCDIR)/%.c)
# - Remove main.o from OBJS as each test comes with its own main
$(TESTS): %: $(OBJDIR)/%.o $(filter-out $(OBJDIR)/main.o,$(OBJS)) libft/libft.a
$(TESTS): tgt_LDLIBS := -lreadline
# - Add tests to OBJS and BINS so static pattern rules with recipes apply. Does
#   not affect OBJS in dependency lists above this line because it is a simply
#   expanded variable, already expanded in there before this assignment.
OBJS += $(TESTS:%=$(OBJDIR)/%.o)
BINS += $(TESTS)
.PHONY: tests
tests: $(TESTS)
all: tests

# libft
INCDIRS += libft
# - Use recursive make for any targets with libft/ prefix
libft/libft.a libft/clean libft/fclean: libft/%:
	+make -C libft $*
# - This Makefile doesn't know whether libft.a needs updating so use a phony
#   target to ensure we always call recursive make. Don't set libft.a directly
#   as phony because that would disable timestamp checks and always relink.
.PHONY: .phony
libft/libft.a: .phony
# - Always call libft clean targets with top level clean targets
.PHONY: libft/clean libft/fclean
clean: libft/clean
fclean: libft/fclean

# Generic utility targets
.DEFAULT_GOAL := all

.PHONY: all clean .clean fclean .fclean re

all: $(NAME)

clean: .clean
.clean:
	rm -rf $(OBJDIR)

fclean: .fclean
.fclean: .clean
	rm -f $(BINS)

re: fclean all

# Default compiler flags that apply to all targets
def_CFLAGS := -Wall -Wextra -Werror -g
def_CPPFLAGS := -MMD -MP $(addprefix -I ,$(INCDIRS))

# Add sanitizer flags if requested
ifneq (,$(strip $(SANITIZE)))
	def_CFLAGS += -fsanitize=$(SANITIZE)
	def_LDFLAGS += -fsanitize=$(SANITIZE)
endif

# Combine default def_FLAGS, target specific tgt_FLAGS and user-supplied FLAGS
# into one _FLAGS variable to be used in recipes
flagvars = CFLAGS CPPFLAGS LDFLAGS LDLIBS
$(foreach v,$(flagvars),$(eval _$v = $$(strip $$(def_$v) $$(tgt_$v) $$($v))))

# Recipe command to ensure directory for target exists
mktargetdir = @mkdir -p $(@D)

# Default recipes for each type of target
$(OBJS): $(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(mktargetdir)
	$(CC) $(_CPPFLAGS) $(_CFLAGS) -c $< -o $@

$(BINS):
	$(mktargetdir)
	$(CC) $(_LDFLAGS) $^ $(_LDLIBS) -o $@

# Inform make that object files don't need to be remade if the requested
# targets are up to date with respect to the source files.
.SECONDARY: $(OBJS)

# Dependency files to handle #include dependencies
DEPS = $(OBJS:.o=.d)
-include $(DEPS)
