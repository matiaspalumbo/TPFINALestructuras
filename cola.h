#ifndef __COLA_H__
#define __COLA_H__

#include <stddef.h>
#include "itree.h"

/* Archivo de cabecera de la implementación de Colas usando 
nodos simplemente enlazados. El tipo de dato es un ITree, y se 
utiliza una estructura auxiliar con el primer y último elemento. */

typedef struct _CNodo {
  ITNodo *dato;
  struct _CNodo *sig;
} CNodo;

typedef struct _Cola {
  CNodo *primero;
  CNodo *ultimo;
} *Cola;


// Crea una nueva cola.
Cola cola_crear();

// Determina si la cola es vacía.
int cola_es_vacia(Cola cola);

// Toma una cola y devuelve el elemento en la primera posición.
ITNodo *cola_primero(Cola cola);

// Toma una cola y un elemento y agrega el elemento al fin de la cola.
void cola_encolar(Cola cola, ITNodo *dato);

// Toma una cola y elimina su primer elemento.
void cola_desencolar(Cola cola);

// Libera la memoria requerida para la cola.
void cola_destruir(Cola cola);


#endif /* __COLA_H__ */
