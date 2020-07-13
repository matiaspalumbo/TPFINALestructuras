#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "gdclist.h"


/* Archivo con la implementación de las funciones sobre listas declaradas en gdclist.h. */


GList gdclist_crear() {
  return NULL;
}

void gdclist_destruir(GList lista, Destruir funcion_destructora) {
  if (!gdclist_es_vacia(lista)) {
    lista->ant->sig = NULL; // Iguala a NULL el siguiente del último elemento de la lista para facilitar la terminación del bucle.
    GNodo *nodoAEliminar;
    while (lista != NULL) { // El bucle se repite hasta que lista llegue a su último elemento.
    // Por cada iteración, elimina el nodo y si es necesario el dato al que apunta.
      nodoAEliminar = lista;
      lista = lista->sig;
      // En caso de ser necesario libera el espacio en memoria del dato del nodo.
      // Si funcion_destructora == NULL, se asume que no es necesario destruir el dato.
      if (funcion_destructora != NULL)
        funcion_destructora(nodoAEliminar->dato);
      // Y luego se libera la memoria del propio nodo.
      free(nodoAEliminar);
    }
  }
}

GList gdclist_vaciar(GList lista, Destruir funcion_destructora) {
  lista->ant->sig = NULL;
  GNodo *nodoAEliminar;
  while (lista != NULL) {
    nodoAEliminar = lista;
    lista = lista->sig;
    if (funcion_destructora != NULL)
      funcion_destructora(nodoAEliminar->dato);
    free(nodoAEliminar);
  }
  return NULL;
}

int gdclist_es_vacia(GList lista) {
  return lista == NULL;
}

int gdclist_longitud(GList lista) {
  int longitud;
  if (gdclist_es_vacia(lista)) longitud = 0; // Si la lista es vacia, su longitud es 0.
  else {
    longitud = 1; // Caso contrario, su longitud es al menos 1.
    GNodo *temp = lista;
    while (temp->sig != lista) {
      // Por cada iteración aumenta la longitud por 1 y avanza un nodo en la lista.
      longitud++;
      temp = temp->sig;
    }
  }
  return longitud;
}

GList gdclist_agregar_inicio(GList lista, void *dato) {
  GNodo *nuevoNodo = malloc(sizeof(GNodo)); // Genera un espacio en la memoria para el nuevo nodo.
  if (gdclist_es_vacia(lista)) { // Si la lista es vacía, 'nuevoNodo' será el único elemento de la lista.
    nuevoNodo->dato = dato;
    nuevoNodo->sig = nuevoNodo;
    nuevoNodo->ant = nuevoNodo;
  } else { // Si la lista no es vacia, modifica los nodos sig. y ant. del nuevo nodo y del primer nodo de la lista para agregarlo.
    nuevoNodo->dato = dato;
    nuevoNodo->sig = lista;
    GNodo *nodo = lista->ant;
    lista->ant = nuevoNodo;
    nodo->sig = nuevoNodo;
    nuevoNodo->ant = nodo;
  }
  return nuevoNodo; // Retorno 'nuevoNodo' ya que pasa a ser el inicio de la lista.
}

GList gdclist_agregar_final(GList lista, void *dato) {
  GNodo *nuevoNodo = malloc(sizeof(GNodo)); // Le asigno un espacio en memoria al nuevo nodo a agregar.
  nuevoNodo->dato = dato;
  if (gdclist_es_vacia(lista)) { // Si la lista es vacía, 'nuevoNodo' será el único elemento de la lista.
    lista = nuevoNodo;
    lista->sig = lista;
    lista->ant = lista;
  } else { // Si la lista no es vacia, modifica los nodos sig. y ant. del nuevo nodo y del último nodo de la lista para agregarlo.
    nuevoNodo->sig = lista;
    GNodo *nodo = lista->ant;
    lista->ant = nuevoNodo;
    nodo->sig = nuevoNodo;
    nuevoNodo->ant = nodo;
  }
  return lista;
}

GList gdclist_intercambiar(GList lista, GNodo *nodo1, GNodo *nodo2) {
  assert(nodo1 != NULL && nodo2 != NULL); // Verifico que los nodos no sean iguales a NULL.
  void *aux = nodo1->dato; // Intercambio los datos.
  nodo1->dato = nodo2->dato;
  nodo2->dato = aux;
  return lista;
}

void gdclist_imprimir(GList lista) {
  GNodo* temp1 = lista;
  printf("%d\n", gdclist_longitud(lista));
  for (int it = 0; it<gdclist_longitud(lista); it++) {
    printf("%d - ", *((int*)temp1->dato));
    temp1 = temp1->sig;
  }
}