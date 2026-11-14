
# ==============================
# Compilateur et options
# ==============================

CC = gcc
SANFLAGS = -fsanitize=address,undefined

CFLAGS = -Wall -Wextra -g $(SANFLAGS)
CPPFLAGS = -Iinclude
LDLIBS =
LDFLAGS = $(SANFLAGS)

# ==============================
# Mode d'exécution
# ==============================

# Mode par défaut
MODE ?= sdl

ifeq ($(MODE),terminal)
    VIEW_SRC = src/view/viewTerminal.c
    CONTROLLER_SRC = src/controller/controllerTerminal.c

else ifeq ($(MODE),sdl)
    VIEW_SRC = src/view/viewSDL.c
    CONTROLLER_SRC = src/controller/controllerSDL.c

    CPPFLAGS += $(shell pkg-config --cflags sdl3)
    LDLIBS += $(shell pkg-config --libs sdl3)

else
    $(error MODE invalide : $(MODE). Utilise MODE=terminal ou MODE=sdl)
endif

# ==============================
# Dossiers
# ==============================

SRC_DIR = src
BUILD_DIR = build/$(MODE)

# ==============================
# Fichiers sources
# ==============================

COMMON_SRC = $(filter-out \
    src/view/viewTerminal.c \
    src/view/viewSDL.c \
    src/controller/controllerTerminal.c \
    src/controller/controllerSDL.c, \
    $(shell find $(SRC_DIR) -name '*.c'))

SRC = $(COMMON_SRC) $(VIEW_SRC) $(CONTROLLER_SRC)

OBJ = $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(SRC))
DEP = $(OBJ:.o=.d)

# ==============================
# Exécutable
# ==============================

EXEC = snake-$(MODE)

.PHONY: all run clean mrproper terminal sdl

terminal:
	$(MAKE) MODE=terminal all

sdl:
	$(MAKE) MODE=sdl all

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# ==============================
# Compilation
# ==============================

$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEP)

# ==============================
# Exécution
# ==============================

run: $(EXEC)
	./$(EXEC)

# ==============================
# Nettoyage
# ==============================

clean:
	rm -rf build

mrproper: clean
	rm -f snake-terminal snake-sdl
