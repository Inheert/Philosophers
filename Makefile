NAME = philosophers.a

CC = cc

CFLAGS = -g3 -Wall -Wextra -Werror

INCLUDES = -I ./includes/

SRC_DIR = .
BIN_DIR = ./bin

SOURCES = src/garbage_collector.c \
			src/manipulation.c \
			src/tracking.c \
			src/ft_malloc.c \
			src/t_ptr_stockage_utils.c \

OBJECTS = $(addprefix $(BIN_DIR)/,$(SOURCES:.c=.o))

all: $(NAME)

$(NAME): $(OBJECTS)
	@$(AR) rcs $@ $^

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

clean:
	@rm -f $(BIN_DIR)/**/*.o

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re