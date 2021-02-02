#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "pila.h"


Pila pila_crear() {
  return NULL;
}


int pila_es_vacia(Pila pila) {
  return pila == NULL;
}


void* pila_ultimo(Pila pila) {
  return pila->dato;
}


Pila pila_apilar(Pila pila, void* dato) {
  PilaSNodo *nuevoElemento = malloc(sizeof(PilaSNodo));
  nuevoElemento->dato = dato;
  nuevoElemento->sig = pila;
  return nuevoElemento;
}


void pila_desapilar(Pila* pila) {
  assert(*pila != NULL);
  Pila nodoAEliminar = *pila;
  *pila = (*pila)->sig;
  free(nodoAEliminar);
}


void pila_destruir(Pila pila) {
  PilaSNodo *elemAEliminar;
  while (pila != NULL) {
    elemAEliminar = pila;
    pila = pila->sig;
    free(elemAEliminar);
  }
}



