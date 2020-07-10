#include <stdlib.h>
#include <stdio.h>
#include "cola.h"

/* Archivo con la implementación de la estructura cola. */


Cola cola_crear() {
  Cola cola = malloc(sizeof(struct _Cola));
  cola->primero = NULL;
  cola->ultimo = NULL;
  return cola;
}

int cola_es_vacia(Cola cola) {
  return cola->ultimo == NULL;
}

ITNodo *cola_primero(Cola cola) {
  return cola->primero->dato;
}

void cola_encolar(Cola cola, ITNodo *dato) {
  /* Crea el nuevo nodo a encolar con el dato pasado. */
  CNodo *nuevoNodo = malloc(sizeof(CNodo));
  nuevoNodo->dato = dato;
  nuevoNodo->sig = NULL;
  /* Si la cola es vacía el nuevo nodo va a ser el primero de la cola. En caso de no serlo, 
  sería el siguiente del último. Una vez ubicado, el nuevo nodo será el último de la cola. */
  if (cola_es_vacia(cola))
    cola->primero = nuevoNodo;
  else
    cola->ultimo->sig = nuevoNodo;
  cola->ultimo = nuevoNodo; 
}

void cola_desencolar(Cola cola) {
  if (!cola_es_vacia(cola)) {
    CNodo *nodoAEliminar = cola->primero; /* Selecciono el nodo a eliminar si la cola no es vacía. */
    if (cola->ultimo == cola->primero) cola->ultimo = NULL; /* Si la cola tiene un solo elemento, queda vacía. */
    cola->primero = cola->primero->sig;
    free(nodoAEliminar); 
  }
}

void cola_destruir(Cola cola) {
  for (CNodo *nodo = cola->primero; nodo != NULL; nodo = nodo->sig)
    free(nodo);
  free(cola);
}