#ifndef __SPILA_H__
#define __SPILA_H__

#include <stddef.h>

typedef struct _PilaSNodo {
  void* dato;
  struct _PilaSNodo *sig;
} PilaSNodo;

/* Se representan pila mediante listas enlazadas.
El comienzo de la lista será el elemento de la cima (el TOPE) de la pila. */

typedef PilaSNodo* Pila;

typedef void (*FuncionImpresora) (void* dato);

// Crea una pila.
Pila pila_crear();

// Determina si una pila es vacía.
int pila_es_vacia(Pila pila);

// Toma una pila y devuelve el elemento de la cima.
void* pila_ultimo(Pila pila);

// Toma una pila y un elemento y agrega el elemento a la pila.
Pila pila_apilar(Pila pila, void* dato) ;

// Toma una pila y elimina el elemento de la cima.
void pila_desapilar(Pila* pila);

// Toma una pila y la imprime en orden.
void pila_imprimir(Pila pila, FuncionImpresora imprimir);

// Libera la memoria requerida para la pila.
void pila_destruir(Pila pila);

#endif /* __SPILA_H__ */