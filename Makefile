
# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -g

# Nom de l'exécutable
EXEC = snake

# Dossier de compilation
BUILD_DIR = build

# Fichiers sources, objets et dépendances
SRC = $(wildcard *.c)
OBJ = $(SRC:%.c=$(BUILD_DIR)/%.o)
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
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

# Chargement des dépendances des fichiers .h
-include $(DEP)

# Compilation et exécution
run: $(EXEC)
	./$(EXEC)

# Suppression des objets et dépendances
clean:
	rm -f $(OBJ) $(DEP)

# Nettoyage complet
mrproper: clean
	rm -f $(EXEC)
