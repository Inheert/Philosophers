NAME = philo

CC = cc

CFLAGS = -g3 -Wall -Wextra -Werror #-fsanitize=thread

INCLUDES = -I ./includes/

SRC_DIR = .
BIN_DIR = ./bin

SOURCES = main.c \
			src/error.c \
			src/utils.c \
			src/preprocessing.c \
			src/thread_manager.c \
			src/thread.c
OBJECTS = $(addprefix $(BIN_DIR)/,$(SOURCES:.c=.o))

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS) $(INCLUDES) -lpthread

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

clean:
	@/bin/rm -rf $(BIN_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
