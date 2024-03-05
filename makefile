##Definir o compilador e suas flags
CFLAGS= -Wall -std=c99 -g
CC = gcc

#Definir os diretorios
OBJ = obj
SRC = src
HEAD = hed

#Definir os arquivos
SRCS = $(wildcard $(SRC)/*.c)
OBJS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))


BINDIR = bin
BIN = $(BINDIR)/COR

all: $(BIN)

#Criar o executavel
$(BIN): $(OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) $(OBJS) -o $@

#Compilar arquivo auxFunc.c
$(OBJ)/auxFunc.o: $(SRC)/auxFunc.c $(HEAD)/COR.h | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

#Compilar os arquivos
$(OBJ)/%.o: $(SRC)/%.c $(HEAD)/%.h | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@


#Criar os diretorios
$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJ):
	mkdir -p $(OBJ)

clean:
	rm -r $(BINDIR)/* $(OBJ)/*

test:
	$(BINDIR)/./COR 127.0.0.1 58002

