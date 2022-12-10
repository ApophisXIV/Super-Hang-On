INC_PATH = ./inc
SRC_PATH = ./src
BUILD_PATH = ./build
OBJ_PATH = $(BUILD_PATH)/obj

PROGRAM = Hang-On
CC = gcc
CFLAGS = -Wall -std=c99 -pedantic -Werror -g -O0 -I $(INC_PATH)
LDFLAGS = -lm -lSDL2

SRC_FILES = $(shell find $(SRC_PATH) -name '*.c')
OBJ_FILES = $(subst $(SRC_PATH),$(OBJ_PATH),$(SRC_FILES:.c=.o))
DEP_FILES = $(subst $(SRC_PATH),$(OBJ_PATH),$(SRC_FILES:.c=.d))

# Lista de directorios donde buscar los archivos .c y .o (https://www.gnu.org/software/make/manual/html_node/General-Search.html)
vpath %.c $(SRC_PATH):$(SRC_PATH)/game:$(SRC_PATH)/game/math:$(SRC_PATH)/game/obj:$(SRC_PATH)/game/phy:$(SRC_PATH)/gfx:$(SRC_PATH)/utils
vpath %.o $(OBJ_PATH):$(OBJ_PATH)/game:$(OBJ_PATH)/game/math:$(OBJ_PATH)/game/obj:$(OBJ_PATH)/game/phy:$(OBJ_PATH)/gfx:$(OBJ_PATH)/utils

#Patterns Rules (https://www.gnu.org/software/make/manual/html_node/Pattern-Rules.html)
%.o: %.c
	@echo -e '\033[1;33m=============================================================================\033[0m'
	@echo -e '\033[1;41m --> \033[0;1;34m Compilando: \033[0m \033[1;33m $< \033[0m\033[0m'
	@echo -e '\033[1;33m=============================================================================\033[0m'
	@mkdir -p $(dir $(OBJ_FILES))
	$(CC) $(CFLAGS) -c $< -o $(subst $(SRC_PATH),$(OBJ_PATH),$(dir $<))$@
	@echo -e '\033[1;33m=============================================================================\n\033[0m'

$(PROGRAM): $(notdir $(OBJ_FILES))
	@echo -e '\033[1;33m=============================================================================\033[0m'
	@echo -e '\033[1;41m --> \033[0;1;34m Linkeando: \033[0m \033[1;33m $@ \033[0m\033[0m'
	@echo -e '\033[1;33m=============================================================================\033[0m'
	$(CC) $(OBJ_FILES) -o $(PROGRAM) $(LDFLAGS)
	@echo -e '\033[1;33m=============================================================================\n\033[0m'


run: $(PROGRAM)
	@echo -e '\033[1;36m=============================================================================\033[0m'
	@echo -e '\033[1;41m --> \033[0;1;34m Ejecutando: \033[0m \033[1;35m $(PROGRAM) \033[0m\033[0m'
	@echo -e '\033[1;36m=============================================================================\033[0m'
	./$(PROGRAM)
	@rm -f $(OBJ_FILES)
	@rm -f $(PROGRAM)
	@echo -e '\033[1;36m=============================================================================\n\033[0m'


clean:
	@echo -e '\033[1;33m=============================================================================\033[0m'
	@echo -e '\033[1;41m --> \033[0;1;34m Limpiando el directorio... \033[0m'
	@echo -e '\033[1;33m=============================================================================\033[0m'
	@echo -e '\033[1;41m Borrando archivos \033[0m'
	rm -f $(OBJ_FILES)
	rm -f $(PROGRAM)
	@echo -e '\033[1;33m=============================================================================\n\033[0m'

info:
	@echo -e '\033[1;33m=============================================================================\033[0m'
	@echo -e '\033[1;41m --> \033[0;1;34m Variables: \033[0m'
	@echo -e '\033[1;33m=============================================================================\033[0m'
	@echo -e '\033[4;32m'SRC_PATH:'\033[0m ' $(SRC_PATH)
	@echo -e '\033[4;32m'INC_PATH:'\033[0m ' $(INC_PATH)
	@echo -e '\033[4;32m'BUILD_PATH:'\033[0m ' $(BUILD_PATH)
	@echo -e '\033[4;32m'OBJ_PATH:'\033[0m ' $(OBJ_PATH)
	@echo -e '\033[4;32m'SRC_FILES:'\033[0m ' $(SRC_FILES)
	@echo -e '\033[4;32m'OBJ_FILES:'\033[0m ' $(OBJ_FILES)
	@echo -e '\033[4;32m'DEP_FILES:'\033[0m ' $(DEP_FILES)
	@echo -e '\033[4;32m'PROGRAM:'\033[0m ' $(PROGRAM)
	@echo -e '\n\033[1;33m=============================================================================\033[0m'