# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include "gdclist.h"
# include "algoritmos.h"



// Función auxiliar que encapsula líneas de codigo que se ejecutan repetidas veces en la función merge para modularizarlo.
void agregar_final_existente(GList resultado, GNodo *lista) {
  resultado->sig = lista;
  lista->ant = resultado;
}


// merge fusiona dos listas ordenadas y devuelve el resultado. Se utiliza como función auxiliar para merge_sort.
GList merge(GList listaL, GList listaR, FuncionComparadora comparar) {
  listaL->ant->sig = NULL; // Se apunta el siguiente del último elem. de cada lista a NULL.
  listaR->ant->sig = NULL;
  GList resultado;
  GNodo *ultimo; // Nodo auxiliar que apuntará al último elemento de listaOrdenada.
  if (comparar(listaL->dato, listaR->dato) <= 0) { // Inicializo la lista a devolver según corresponda.
    resultado = listaL;
    listaL = listaL->sig;
  } else {
    resultado = listaR;
    listaR = listaR->sig;
  }
  GList listaOrdenada = resultado; // Lista a devolver.
  // Mientras las dos listas sean no vacías, las fusiona según el orden de los elementos.
  while (!gdclist_es_vacia(listaL) && !gdclist_es_vacia(listaR)) {
    if (comparar(listaL->dato, listaR->dato) <= 0) {
      agregar_final_existente(resultado, listaL);
      listaL = listaL->sig;
    } else {
      agregar_final_existente(resultado, listaR);
      listaR = listaR->sig;
    }
    resultado = resultado->sig;
  }
  // A lo sumo uno de los dos siguiente bucles se ejecutarán dependiendo de la lista de mayor longitud.
  // Se agregan los elementos restantes.
  while (!gdclist_es_vacia(listaL)) {
      agregar_final_existente(resultado, listaL);
      if (listaL->sig == NULL) {
        ultimo = listaL;
        listaL = listaL->sig;
      } else {
        listaL = listaL->sig;
        resultado = resultado->sig;
      }
  }
  while (!gdclist_es_vacia(listaR)) {
      agregar_final_existente(resultado, listaR);
      if (listaR->sig == NULL) {
        ultimo = listaR;
        listaR = listaR->sig;
      } else {
        listaR = listaR->sig;
        resultado = resultado->sig;
      }
  }
  agregar_final_existente(ultimo, listaOrdenada); // Se apunta el último nodo al primero de la lista a devolver.
  return listaOrdenada;
}

/* Merge Sort ordena una lista dividiendo sus elementos en dos mitades, llamándose a sí misma recursivamente 
y luego fusionando las dos sublistas ordenadas resultantes. El caso base del algoritmo sucede cuando la lista es unitaria. */
GList merge_sort(GList lista, FuncionComparadora comparar) {
  GList listaOrdenada;
  int longitud  = gdclist_longitud(lista);
  if (longitud == 1 || gdclist_es_vacia(lista)) // Caso base.
    listaOrdenada = lista;
  else {
    GList izqOrdenado = lista;
    GList derOrdenado;
    GNodo* temp = lista;
    int mitad = (longitud % 2 == 0) ? (longitud / 2) : (longitud / 2 + 1); // mitad indica la posición del primer elemento de la sublista derecha.
    for (int i = 0; i < mitad-1; i++) {
      temp = temp->sig;
    } // Ahora temp apunta al último nodo de la sublista izquierda.
    derOrdenado = temp->sig; // Se guardan las posiciones a reemplazar en izqOrdenado para derOrdenado.
    GNodo *finalLista2 = izqOrdenado->ant;
    temp->sig = izqOrdenado;
    izqOrdenado->ant = temp;
    finalLista2->sig = derOrdenado;
    derOrdenado->ant = finalLista2;
    // Ya quedaron determinadas las dos sublistas, por lo que ahora sucede la recursión.
    izqOrdenado = merge_sort(izqOrdenado, comparar);
    derOrdenado = merge_sort(derOrdenado, comparar);
    listaOrdenada = merge(izqOrdenado, derOrdenado, comparar); // Se fusionan las sublistas ordenadas mediante la función auxiliar merge.
  }
  return listaOrdenada;
}
