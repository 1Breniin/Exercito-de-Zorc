# Nome do executável
EXEC = tp2

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -O2

# Arquivos fonte
SRCS = main.c grafo.c recrutamento.c utils.c

# Arquivos de cabeçalho
HDRS = grafo.h recrutamento.h utils.h

# Objetos gerados
OBJS = $(SRCS:.c=.o)

# Regra principal
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Regra para compilar arquivos .c em .o
%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpa arquivos objetos e executável
clean:
	rm -f $(OBJS) $(EXEC)

# Limpa tudo inclusive a saída
clean-all: clean
	rm -f saida.txt

# Instala o executável no diretório raiz
install: $(EXEC)
	cp $(EXEC) ..