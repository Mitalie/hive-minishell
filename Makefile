# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/11 15:47:17 by amakinen          #+#    #+#              #
#    Updated: 2025/02/18 18:07:26 by amakinen         ###   ########.fr        #
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
)

OBJS := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
BINS := $(NAME)
$(NAME): $(OBJS) libft/libft.a

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
