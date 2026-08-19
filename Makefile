NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

M_OBj =	src/codexion.c \
		src/helper_functions.c \
		src/parser.c \
		
		


OBJ = $(M_OBj:.c=.o)


all: $(NAME)

$(NAME): $(OBJ) src/codexion.h
	$(CC) $(CFLAGS) $(M_OBj) -o $(NAME)

clean:
	rm -rf $(OBJ)

fclean:
	rm -rf $(OBJ) $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re