#ifndef __GDCLIST_H__
#define __GDCLIST_H__

#include <stddef.h>

// Archivo de cabecera de la implementación de listas generales doblemente enlazadas circulares.


typedef struct _GNodo { // Estructura de un nodo de una lista general.
  void *dato;
  struct _GNodo *sig;
  struct _GNodo *ant;
} GNodo;

typedef GNodo *GList; // Estructura de una lista general.

/* Puntero a una función que toma como argumneto un puntero a void y retorna void.
Las funciones de este tipo se utilizarán para recorrer cada nodo de una lista. */
typedef void (*FuncionVisitante) (void *dato);

/* Puntero a una función que toma como argumento un puntero a void y retorna void. 
Funciones de este tipo serán utilizadas para eliminar el dato presente en un nodo de la lista.*/
typedef void (*Destruir) (void *dato);


// Devuelve una lista vacía.
GList gdclist_crear();

// Destruye la lista dada y, en caso de que sea necesario, el dato que guarda cada nodo.
void gdclist_destruir(GList lista, Destruir funcion_destructora);

GList gdclist_vaciar(GList lista, Destruir funcion_destructora);

// Determina si la lista es vacía.
int gdclist_es_vacia(GList lista);

// Devuelve la longitud de una lista.
int gdclist_longitud(GList lista);

// Agrega un elemento al inicio de una lista. 
GList gdclist_agregar_inicio(GList lista, void *dato);

// Agrega un elemento al final de una lista.
GList gdclist_agregar_final(GList lista, void *dato);

// Dada una lista y dos nodos de ella, intercambia los datos de los nodos entre sí.
GList gdclist_intercambiar(GList lista, GNodo *nodo1, GNodo *nodo2);

void gdclist_imprimir(GList lista);

#endif /* __GDCLIST_H */