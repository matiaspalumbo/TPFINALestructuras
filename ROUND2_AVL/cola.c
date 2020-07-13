#include "cola.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>

/*
typedef struct _Cola {
  ColaSNodo* primero;
  ColaSNodo* ultimo;
} *Cola;
*/


Cola cola_crear() {
  Cola cola = malloc(sizeof(Cola));
  cola->primero = NULL;
  cola->ultimo = NULL;
  return cola;
}


// Determina si la cola está vacía.
int cola_es_vacia(Cola cola) {
  return cola->primero == NULL;
}


void* cola_primero(Cola cola) {
  assert(!cola_es_vacia(cola));
  return cola->primero->dato;
}

void cola_encolar(Cola cola, void* dato) {
  ColaSNodo* nuevoNodo = malloc(sizeof(ColaSNodo));
  nuevoNodo->dato = dato;
  nuevoNodo->sig = NULL;
  if (cola_es_vacia(cola))
    cola->primero  = nuevoNodo;
  else
    cola->ultimo->sig = nuevoNodo;
  cola->ultimo = nuevoNodo;
}

void cola_desencolar(Cola cola) {
  ColaSNodo* elemAEliminar = cola->primero;
  cola->primero = cola->primero->sig;
  free(elemAEliminar);
}

void cola_imprimir(Cola cola, FuncionImpresora imprimir) {
  ColaSNodo* temp = cola->ultimo;
  for (; temp->sig != NULL; temp = temp->sig);
  ColaSNodo* toStop = temp;
  while (temp != cola->ultimo) {
    imprimir(temp->dato);
    temp = cola->ultimo;
    for (; temp->sig != toStop; temp = temp->sig);
    toStop = temp;
  }
  imprimir(cola->ultimo->dato);
  puts("");




  for (ColaSNodo* elem = cola->primero; elem != NULL; elem = elem->sig)
      imprimir(elem->dato);
  puts("");
}

void cola_destruir(Cola cola) {
  ColaSNodo *elemAEliminar;
  while (cola->primero != NULL) {
    elemAEliminar = cola->primero;
    cola->primero = cola->primero->sig;
    free(elemAEliminar);
  }
  free(cola);
}




