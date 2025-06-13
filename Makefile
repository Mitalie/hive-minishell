# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/11 15:47:17 by amakinen          #+#    #+#              #
#    Updated: 2025/06/13 18:50:47 by amakinen         ###   ########.fr        #
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
	input.c \
	input_notty.c \
	signals.c \
	shutil.c \
	status.c \
	tokenizer.c \
	ast/ast_free.c \
	builtin/builtin.c \
	builtin/builtin_cmd_exit.c \
	builtin/builtin_cmd_echo.c \
	builtin/builtin_cmd_pwd.c \
	builtin/builtin_cmd_cd.c \
	builtin/builtin_cmd_env.c \
	builtin/builtin_cmd_export.c \
	builtin/builtin_cmd_export_utils.c \
	builtin/builtin_cmd_unset.c \
	parser/parser.c \
	parser/parser_syntax_error.c \
	parser/parser_list.c \
	parser/parser_group.c \
	parser/parser_pipeline.c \
	parser/parser_simple_command.c \
	parser/parser_redirect.c \
	parser/parser_word.c \
	parser/parser_heredoc.c \
	execute/execute_list.c \
	execute/execute_pipeline.c \
	execute/execute_simple_command.c \
	execute/execute_external.c \
	execute/execute_redirect.c \
	execute/execute_heredoc.c \
	util/util_char.c \
	util/util_parse_int.c \
	util/util_write.c \
	util/util_itoa.c \
	word/word.c \
	word/word_exp.c \
	word/word_filename.c \
	word/word_heredoc.c \
	word/word_pattern.c \
	word/word_scan.c \
	word/word_unescape.c \
	shenv/shenv_init.c \
	shenv/shenv_var_get_set.c \
	shenv/shenv_var_utils.c \
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
	wordexp \
	wordhere \
	env \
	path_execve \
	heredoc \
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
