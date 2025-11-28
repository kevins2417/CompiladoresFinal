# Makefile con targets dinámicos
CC = gcc
LEX = flex
CFLAGS = -Wall -g -std=c99 -D_POSIX_C_SOURCE=200809L
LDFLAGS = -lfl

TARGET = mini0parser
OBJS = main.o parser.o ll1.o tokens.o lex.yy.o

.PHONY: all clean help

# Compilar
all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "🔨 Compilando $(TARGET)..."
	@$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

lex.yy.c: lexer.l
	@$(LEX) lexer.l

%.o: %.c
	@$(CC) $(CFLAGS) -c $<

# Limpiar
clean:
	@echo "🧹 Limpiando..."
	@rm -f $(TARGET) $(OBJS) lex.yy.c

# Ayuda
help:
	@echo "Comandos:"
	@echo "  make              - Compilar"
	@echo "  make clean        - Limpiar"
	@echo "  make <archivo.mini0> - Ejecutar parser con ese archivo"
	@echo ""
	@echo "Archivos .mini0 disponibles:"
	@ls *.mini0 2>/dev/null || echo "  (No hay archivos .mini0)"

# Target dinámico para ANY archivo .mini0
%.mini0: $(TARGET)
	@echo "🚀 Ejecutando: ./$(TARGET) $@"
	@./$(TARGET) $@

.DEFAULT_GOAL := all