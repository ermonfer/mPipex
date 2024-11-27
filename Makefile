NAME 	:= pipex
LIBFT_DIR := ./libft/
LIBFT := $(LIBFT_DIR)/libft.a


#Compilador y Flags
CC	:= gcc
CFLAGS	:= -Wall -Werror -Wextra

#Archivos fuente y directorios
# TODO Uso wildcard por flexibilidad pero la norma obliga a incluir explicitamente. Hay que cambiarlo al final.
DIR_MANDATORY	:= mandatory
SRC_MAN	:= $(wildcard $(DIR_MANDATORY)/*.c)
OBJ_MAN:= ${SRC_MAN:.c=.o}
DIR_BONUS	:= bonus
SRC_BONUS	:= $(wildcard $(DIR_BONUS)/*.c)
OBJ_BONUS:= ${SRC_BONUS:.c=.o}
 #Headers
HEADERS := -I./libft/include

#Librerias 
LIBS := -L./libft/

#Reglas de compilación

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ_MAN)
	$(CC) $(CFLAGS) -o $@ $(OBJ_MAN) $(HEADERS) $(LIBS)

bonus: $(LIBFT_DIR)/Makefile .cache_bonus
	@touch $(OUTDIR)/.cache_bonus

cache_bonus: $(LIBFT) $(OBJ_BONUS) 
	$(CC) $(CFLAGS) -o $(NAMe) $(OBJ_BONUS) $(HEADERS) $(LIBS)
	
$(LIBFT):
	git submodule update --init --recursive
	make -C $(LIBFT_DIR)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

# Limpiar los archivos y el ejecutable
clean:
	make -C $(LIBFT_DIR) clean
	rm -f $(OBJ_MAN) $(OBJ_BONUS)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)
	rm -f .cache_bonus

re: fclean $(NAME)

.PHONY: all bonus clean fclean re
