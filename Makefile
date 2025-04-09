# Nome do compilador
CC = gcc

# Flags do compilador
CFLAGS = -Wall -Wextra -std=c99

# Nome do executável
EXEC = simulador

# Arquivos fonte
SRC = main.c scheduler.c input.c process_generator.c stats.c utils.c

# Arquivos objeto
OBJ = $(SRC:.c=.o)

# Arquivos de cabeçalho
HEADERS = process.h scheduler.h input.h process_generator.h stats.h utils.h

# Regra padrão para compilar o programa
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ) -lm

# Regra para compilar os arquivos .c em .o
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

# Limpar arquivos gerados
.PHONY: clean
clean:
	rm -f $(OBJ) $(EXEC)

# Regra para criar tudo novamente
.PHONY: all
all: clean $(EXEC)

