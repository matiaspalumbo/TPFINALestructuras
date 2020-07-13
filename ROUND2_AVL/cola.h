#ifndef __SCOLA_H__
#define __SCOLA_H__

#include <stddef.h>

typedef struct _ColaSNodo {
  void* dato;
  struct _ColaSNodo *sig;
} ColaSNodo;

/* Se representarán colas mediante listas enlazadas y una estructura auxiliar
con el primer y último elemento. El primer elemento de la cola es el final de 
la lista, y el último elemento es el principio. */

typedef struct _Cola {
	ColaSNodo* primero;
	ColaSNodo* ultimo;
} *Cola;

typedef void (*FuncionImpresora) (void* dato);


// Crea una nueva cola.
Cola cola_crear();

// Determina si la cola está vacía.
int cola_es_vacia(Cola cola);

// Toma una cola y devuelve el elemento en la primera posición.
void* cola_primero(Cola cola);

// Toma una cola y un elemento y agrega el elemento al fin de la cola.
void cola_encolar(Cola cola, void* dato);

// Toma una cola y elimina su primer elemento.
void cola_desencolar(Cola cola);

// Toma una cola y la imprime en orden.
void cola_imprimir(Cola cola, FuncionImpresora imprimir);

// Libera la memoria requerida para la cola.
void cola_destruir(Cola cola);



#endif /* __SCOLA_H__ */
