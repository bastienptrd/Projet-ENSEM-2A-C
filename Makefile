
# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -g
CPPFLAGS = -Iinclude
LDFLAGS =
LDLIBS =

# Nom de l'exécutable
EXEC = snake

# Dossiers
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
ASSETS_DIR = assets

# Fichiers sources, objets et dépendances
SRC = $(wildcard $(SRC_DIR)/*.c)
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
