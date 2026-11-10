
# ==============================
# Compilateur et options
# ==============================

CC = gcc
CFLAGS = -Wall -Wextra -g
CPPFLAGS = -Iinclude
LDLIBS =
LDFLAGS =

# ==============================
# Mode d'exécution
# ==============================

# Mode par défaut
MODE ?= sdl

ifeq ($(MODE),terminal)
    VIEW_SRC = src/viewTerminal.c
    CONTROLLER_SRC = src/controllerTerminal.c

else ifeq ($(MODE),sdl)
    VIEW_SRC = src/viewSDL.c
    CONTROLLER_SRC = src/controllerSDL.c

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
    src/viewTerminal.c \
    src/viewSDL.c \
    src/controllerTerminal.c \
    src/controllerSDL.c, \
    $(wildcard src/*.c))

SRC = $(COMMON_SRC) $(VIEW_SRC) $(CONTROLLER_SRC)

OBJ = $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(SRC))
DEP = $(OBJ:.o=.d)

# ==============================
# Exécutable
# ==============================

EXEC = snake-$(MODE)

.PHONY: all run clean mrproper

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# ==============================
# Compilation
# ==============================

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
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
