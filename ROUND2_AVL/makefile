# Makefile para el Trabajo Practico Final de Estructuras de Datos y Algoritmos I

# Macros.
PILA = pila.c
COLA = cola.c
SETS = sets.c
ITREE = itree.c
ALGORITMOS = algoritmos.c
GDCLIST = gdclist.c
INTERPRETE = interprete.c
TABLAHASH = tablahash.c
FLAGS = -Wall -Wextra -Werror -std=c99 -g

# CAMBIAAAAR
# Ejecutable del Interprete.
interprete : main.c $(COLA:.c=.o) $(PILA:.c=.o) $(INTERPRETE:.c=.o) $(TABLAHASH:.c=.o) $(ITREE:.c=.o) # $(SETS:.c=.o)
	gcc $(FLAGS) -o $@ $^

# Archivo objeto del Interprete.
$(INTERPRETE:.c=.o) : $(INTERPRETE) $(INTERPRETE:.c=.h) $(TABLAHASH:.c=.h)
	gcc $(FLAGS) -c $(INTERPRETE)

# Archivo objeto de la implementación de Tablas Hash.
$(TABLAHASH:.c=.o) : $(TABLAHASH) $(TABLAHASH:.c=.h) $(PILA:.c=.h)
	gcc $(FLAGS) -c $(TABLAHASH)

# Archivo objeto de la implementación de Listas Circulares Doblemente Enlazadas.
$(ITREE:.c=.o) : $(ITREE) $(ITREE:.c=.h)
	gcc $(FLAGS) -c $(ITREE)

# # Archivo objeto de la implementación de Sets.
# $(SETS:.c=.o) : $(SETS) $(SETS:.c=.h)
# 	gcc $(FLAGS) -c $(SETS)

# Archivo objeto de la implementación de Colas.
$(COLA:.c=.o) : $(COLA) $(COLA:.c=.h)
	gcc $(FLAGS) -c $(COLA)

# Archivo objeto de la implementación de Pilas.
$(PILA:.c=.o) : $(PILA) $(PILA:.c=.h)
	gcc $(FLAGS) -c $(PILA)

# Destino simbólico que borra todos los archivos con extensión .o cuando se invoca.
clean: 
	rm *.o

.PHONY : clean
