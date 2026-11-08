
# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -g
CPPFLAGS = -Iinclude
CPPFLAGS += $(shell pkg-config --cflags sdl3)
LDLIBS += $(shell pkg-config --libs sdl3)

# Nom de l'exécutable
EXEC = snake

# Dossiers
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
ASSETS_DIR = assets

# Fichiers sources, objets et dépendances
VIEW_SRC = src/viewTerminal.c
CONTROLLER_SRC = src/controllerTerminal.c
#VIEW_SRC = src/viewSDL.c
#CONTROLLER_SRC = src/controllerSDL.c
SRC = $(filter-out src/viewTerminal.c src/viewSDL.c src/controllerSDL.c src/controllerTerminal.c,$(wildcard src/*.c)) $(VIEW_SRC) $(CONTROLLER_SRC)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))
DEP = $(OBJ:.o=.d)

# Cibles
.PHONY: all run clean mrproper

all: $(EXEC)

# Edition des liens
$(EXEC): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Création du dossier de compilation
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compilation des fichiers .c en fichiers .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

# Chargement des dépendances des fichiers .h
-include $(DEP)

# Compilation et exécution
run: $(EXEC)
	./$(EXEC)

# Suppression des objets et dépendances
clean:
	rm -rf $(BUILD_DIR)

# Nettoyage complet
mrproper: clean
	rm -f $(EXEC)
