# Makefile para el Trabajo Practico Final de Estructuras de Datos y Algoritmos I

# Macros.
PILA = pila.c
ITREE = itree.c
SET = set.c
INTERVALO = intervalo.c
TABLAHASH = tablahash.c
ESTADO = estado.c
INTERPRETE = interprete.c
FLAGS = -Wall -Wextra -Werror -std=c99 -g


# Ejecutable del intérprete.
interprete : main.c $(PILA:.c=.o) $(INTERVALO:.c=.o) $(ITREE:.c=.o) $(SET:.c=.o) $(TABLAHASH:.c=.o) $(ESTADO:.c=.o) $(INTERPRETE:.c=.o)
	gcc $(FLAGS) -o $@ $^

# Archivo objeto del Intérprete.
$(INTERPRETE:.c=.o) : $(INTERPRETE) $(INTERPRETE:.c=.h) $(ESTADO:.c=.h)
	gcc $(FLAGS) -c $(INTERPRETE)

# Archivo objeto del Estado.
$(ESTADO:.c=.o) : $(ESTADO) $(ESTADO:.c=.h) $(TABLAHASH:.c=.h) $(SET:.c=.h)
	gcc $(FLAGS) -c $(ESTADO)

# Archivo objeto de la implementación de Tablas Hash.
$(TABLAHASH:.c=.o) : $(TABLAHASH) $(TABLAHASH:.c=.h) $(PILA:.c=.h)
	gcc $(FLAGS) -c $(TABLAHASH)

# Archivo objeto de la implementación de Sets.
$(SET:.c=.o) : $(SET) $(SET:.c=.h) $(ITREE:.c=.h)
	gcc $(FLAGS) -c $(SET)

# Archivo objeto de la implementación de árboles AVL de intervalos.
$(ITREE:.c=.o) : $(ITREE) $(ITREE:.c=.h) $(INTERVALO:.c=.h) $(PILA:.c=.h)
	gcc $(FLAGS) -c $(ITREE)

# Archivo objeto de la implementación de Pilas.
$(PILA:.c=.o) : $(PILA) $(PILA:.c=.h)
	gcc $(FLAGS) -c $(PILA)

# Archivo objeto de la implementación de Intervalos.
$(INTERVALO:.c=.o) : $(INTERVALO) $(INTERVALO:.c=.h)
	gcc $(FLAGS) -c $(INTERVALO)


# Destino simbólico que borra todos los archivos con extensión .o cuando se invoca.
clean: 
	rm *.o

.PHONY : clean
