NAME = philo
CC = cc
FLAGS = -Wall -Wextra -Werror -g -MMD -MP

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include
LIBFT_DIR = libft

SRC = $(SRC_DIR)/main.c \
	  $(SRC_DIR)/routine.c \
	  $(SRC_DIR)/states.c \
	  $(SRC_DIR)/parse_arguments.c

OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o) \

DEP = $(OBJ=.o=.d)

INC_FLAGS = -I$(INC_DIR)

all: $(OBJ_DIR) $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(FLAGS) $(INC_FLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

-include $(DEP)

.PHONY: all clean fclean re
