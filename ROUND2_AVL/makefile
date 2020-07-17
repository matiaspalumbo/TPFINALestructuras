# Makefile para el Trabajo Practico Final de Estructuras de Datos y Algoritmos I

# Macros.
PILA = pila.c
SET = set.c
TABLAHASH = tablahash.c
ESTADO = estado.c
INTERPRETE = interprete.c
FLAGS = -Wall -Wextra -Werror -std=c99 -g


# Ejecutable del Interprete.
interprete : main.c $(PILA:.c=.o) $(SET:.c=.o)  $(TABLAHASH:.c=.o) $(ESTADO:.c=.o) $(INTERPRETE:.c=.o)
	gcc $(FLAGS) -o $@ $^

# Archivo objeto del Interprete.
$(INTERPRETE:.c=.o) : $(INTERPRETE) $(INTERPRETE:.c=.h) $(TABLAHASH:.c=.h)
	gcc $(FLAGS) -c $(INTERPRETE)

# Archivo objeto del Estado.
$(ESTADO:.c=.o) : $(ESTADO) $(ESTADO:.c=.h) $(TABLAHASH:.c=.h) $(SET:.c=.h)
	gcc $(FLAGS) -c $(ESTADO)

# Archivo objeto de la implementación de Tablas Hash.
$(TABLAHASH:.c=.o) : $(TABLAHASH) $(TABLAHASH:.c=.h) $(PILA:.c=.h)
	gcc $(FLAGS) -c $(TABLAHASH)

$(SET:.c=.o) : $(SET) $(SET:.c=.h)
	gcc $(FLAGS) -c $(SET)

# Archivo objeto de la implementación de Pilas.
$(PILA:.c=.o) : $(PILA) $(PILA:.c=.h)
	gcc $(FLAGS) -c $(PILA)

# Destino simbólico que borra todos los archivos con extensión .o cuando se invoca.
clean: 
	rm *.o

.PHONY : clean
