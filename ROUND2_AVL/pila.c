#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "pila.h"


// Crea una pila.
Pila pila_crear() {
  return NULL;
}


// Determina si una pila es vacía.
int pila_es_vacia(Pila pila) {
  return pila == NULL;
}


// Toma una pila y devuelve el elemento en la cima.
void* pila_ultimo(Pila pila) {
  return pila->dato;
}


// Toma una pila y un elemento y agrega el elemento a la pila.
Pila pila_apilar(Pila pila, void* dato) {
  PilaSNodo *nuevoElemento = malloc(sizeof(PilaSNodo));
  nuevoElemento->dato = dato;
  nuevoElemento->sig = pila;
  return nuevoElemento;
}


// Toma una pila y elimina el elemento de la cima.
void pila_desapilar(Pila* pila) {
  assert(*pila != NULL);
  free(*pila);
  *pila = (*pila)->sig;
}


// Toma una pila y la imprime en orden.
void pila_imprimir(Pila pila, FuncionImpresoraPila imprimir) {
  for (PilaSNodo* temp = pila; temp != NULL; temp = temp->sig) {
    imprimir(temp->dato);
  }
  puts("");
}
// void pila_imprimir(Pila pila, FuncionImpresoraPila imprimir) {
//   PilaSNodo* temp = pila;
//   for (; temp->sig != NULL; temp = temp->sig);
//   PilaSNodo* toStop = temp;
//   while (temp != pila) {
//     imprimir(temp->dato);
//     temp = pila;
//     for (; temp->sig != toStop; temp = temp->sig);
//     toStop = temp;
//   }
//   imprimir(pila->dato);
//   puts("");
// }


// Libera la memoria requerida para la pila.
void pila_destruir(Pila pila) {
  PilaSNodo *elemAEliminar;
  while (pila != NULL) {
    elemAEliminar = pila;
    pila = pila->sig;
    free(elemAEliminar);
  }
}








