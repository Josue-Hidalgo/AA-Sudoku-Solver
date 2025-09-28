# Nombre del compilador
CC = gcc

# Opciones de compilación
CFLAGS = -Wall -Wextra -std=c11

# Librerías
LIBS = -lm

# Nombre del ejecutable
TARGET = prueba

# Archivos fuente
SRC = prueba.c

# Regla por defecto
all: $(TARGET)

# Cómo construir el ejecutable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

# Ejecutar el programa
run: $(TARGET)
	./$(TARGET)

# Limpiar los archivos generados
clean:
	rm -f $(TARGET)
