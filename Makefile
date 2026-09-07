.PHONY: all clean fclean re bonus
.DEFAULT_GOAL: all

CC = cc
CFLAGS = -Wall -Werror -Wextra -pthread

NAME = codexion
SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=%.o)


$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


all: $(NAME)

clean:
	rm -rf *.o

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)


