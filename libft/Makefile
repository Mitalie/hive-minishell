# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amakinen <amakinen@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/16 17:07:39 by amakinen          #+#    #+#              #
#    Updated: 2024/06/26 14:00:43 by amakinen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BUILDDIR = build

_CFLAGS = -Wall -Wextra -Werror $(CFLAGS)
_CPPFLAGS = -MMD -MP $(CPPFLAGS)
CC ?= cc
AR ?= ar
mktargetdir = @mkdir -p $(@D)

NAME = libft.a

SRCS = \
	ft_isalpha.c \
	ft_isdigit.c \
	ft_isalnum.c \
	ft_isascii.c \
	ft_isprint.c \
	ft_strlen.c \
	ft_memset.c \
	ft_bzero.c \
	ft_memcpy.c \
	ft_memmove.c \
	ft_strlcpy.c \
	ft_strlcat.c \
	ft_toupper.c \
	ft_tolower.c \
	ft_strchr.c \
	ft_strrchr.c \
	ft_strncmp.c \
	ft_memchr.c \
	ft_memcmp.c \
	ft_strnstr.c \
	ft_atoi.c \
	ft_calloc.c \
	ft_strdup.c \
	ft_substr.c \
	ft_strjoin.c \
	ft_strtrim.c \
	ft_split.c \
	ft_itoa.c \
	ft_strmapi.c \
	ft_striteri.c \
	ft_putchar_fd.c \
	ft_putstr_fd.c \
	ft_putendl_fd.c \
	ft_putnbr_fd.c \

BONUS_SRCS = \
	ft_lstnew_bonus.c \
	ft_lstadd_front_bonus.c \
	ft_lstsize_bonus.c \
	ft_lstlast_bonus.c \
	ft_lstadd_back_bonus.c \
	ft_lstdelone_bonus.c \
	ft_lstclear_bonus.c \
	ft_lstiter_bonus.c \
	ft_lstmap_bonus.c \

OBJS = $(SRCS:%.c=$(BUILDDIR)/%.o)
BONUS_OBJS = $(BONUS_SRCS:%.c=$(BUILDDIR)/%.o)
DEPS = $(OBJS:.o=.d) $(BONUS_OBJS:.o=.d)

# Inform make that .o files don't need to be remade if the actual target
# (e.g. libft.a) is up to date with respect to the source files.
.SECONDARY: $(OBJS) $(BONUS_OBJS)

.PHONY: all clean fclean re bonus

all: $(NAME)

clean:
	rm -rf $(BUILDDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

$(NAME): $(OBJS)
	$(AR) -crs $@ $?

# Bonus doesn't create a its own target file which make could use to determine
# whether the it needs updating, so instead we target a dummy archive member
# which specifies the actual dependencies and the ar recipe.
bonus: $(NAME)(.bonus-timestamp)

# Some Linux distros configure ar to enable deterministic mode by default, which
# breaks make's archive member checks and causes our bonus target to repack even
# without changes. Use -U flag to disable, but only on Linux as not all ar
# implementations understand the -U flag.
ifeq ($(shell uname -s),Linux)
AR_U ?= -U
AR += $(AR_U)
endif

# Order-only dependency on the non-bonus part ensures make can't corrupt the
# archive by executing both ar recipes modifying it simultaneously.
$(NAME)(.bonus-timestamp): $(BONUS_OBJS) | $(NAME)
	@touch .bonus-timestamp
	$(AR) -crs $(NAME) .bonus-timestamp $?
	@rm .bonus-timestamp

$(BUILDDIR)/%.o: %.c
	$(mktargetdir)
	$(CC) $(_CFLAGS) $(_CPPFLAGS) -c $< -o $@

-include $(DEPS)
