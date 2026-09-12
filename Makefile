# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aezzirar <aezzirar@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/09/09 13:40:33 by aezzirar          #+#    #+#              #
#    Updated: 2026/09/12 14:56:56 by aezzirar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = codexion
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -pthread
RM          = rm -rf

SRCS_DIR    = srcs
OBJS_DIR    = objs

SRCS        = $(SRCS_DIR)/main.c \
              $(SRCS_DIR)/init.c \
              $(SRCS_DIR)/heap.c \
              $(SRCS_DIR)/heap_ops.c \
              $(SRCS_DIR)/coder.c \
              $(SRCS_DIR)/coder_wait.c \
              $(SRCS_DIR)/monitor.c \
              $(SRCS_DIR)/time.c \
              $(SRCS_DIR)/utils.c

OBJS        = $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

clean:
	$(RM) $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re