#ifndef __ITREE_H__
#define __ITREE_H__

#include "intervalo.h"

/* Archivo de cabecera de la implementación de árboles AVL de intervalos. */


typedef struct _ITNodo {
  Intervalo* intv; // Intervalo del nodo.
  int max, min, altura; // Extremo derecho máximo, izquierdo mínimo y altura del nodo, respectivamente.
  struct _ITNodo *right; // Hijo derecho.
  struct _ITNodo *left; // Hijo izquierdo.
} ITNodo;

typedef ITNodo* ITree;


/* Crea un ITree vacío. */
ITree itree_crear();

/* Destruye un ITree. */
void itree_destruir(ITree nodo);

/* Verifica si el ITree dado es vacío. */
int itree_empty(ITree nodo);

/* Agrega un intervalo al ITree dado. */
ITree itree_insertar(ITree nodo, Intervalo *intv);

/* Elimina (si existe) un nodo del árbol con el intervalo dado. */
ITree itree_eliminar(ITree nodo, Intervalo *intv);


#endif /* __ITREE_H__ */
