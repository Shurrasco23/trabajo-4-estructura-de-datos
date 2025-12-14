# Compilador
CC     = gcc

# Flags de compilación (puedes ajustar)
CFLAGS = -Wall -Wextra -std=c11 -g

# Nombre del ejecutable
TARGET = pvv

# Archivos fuente
SRCS   = pvv.c commands.c structure.c

# Archivos objeto (a partir de los .c)
OBJS   = $(SRCS:.c=.o)

# Regla por defecto: compilar todo
all: $(TARGET)

# Cómo enlazar el ejecutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Cómo compilar cada .c a .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar archivos generados
clean:
	rm -f $(OBJS) $(TARGET)

# Regla opcional para ejecutar rápido con argumentos de prueba
run: $(TARGET)
	./$(TARGET) help
