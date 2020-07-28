#include "itree.h"
#include "pila.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

/* Archivo con la implementación de árboles AVL de intervalos. */


ITree itree_crear() {
  return NULL;
}


void itree_destruir(ITree nodo) {
  if (!itree_empty(nodo)) {
    itree_destruir(nodo->left);
    itree_destruir(nodo->right);
    intv_destruir(nodo->intv);
    free(nodo);
  }
}


int itree_empty(ITree nodo) {
  return nodo == NULL;
}


/* Devuelve la altura del nodo. */
int itree_altura(ITree nodo) {
  return (itree_empty(nodo)) ? -1 : nodo->altura;
}


/* Devuelve el factor de balance del nodo. */
int itree_balance_factor(ITree nodo) {
  return itree_altura(nodo->left) - itree_altura(nodo->right);
}


/* Calcula el máximo entre dos enteros. */
int maximo(int n1, int n2) {
  return n1 < n2 ? n2 : n1;
}

/* Calcula el mínimo entre dos enteros. */
int minimo(int n1, int n2) {
  return n1 < n2 ? n1 : n2;
}


/* Calcula el máximo extremo derecho de un ITree luego de insertar un nodo. */
int calcular_max(ITree nodo) {
  if (!itree_empty(nodo->left) && !itree_empty(nodo->right))
    return maximo(maximo(nodo->intv->der, nodo->left->max), nodo->right->max);
  else if (!itree_empty(nodo->left) || !itree_empty(nodo->right)) {
    ITree nodoNoNULL = (!itree_empty(nodo->left)) ? nodo->left : nodo->right;
    return maximo(nodo->intv->der, nodoNoNULL->max);
  } else
    return nodo->intv->der;
}


/* Calcula el mínimo extremo izquierdo de un ITree luego de insertar un nodo. */
int calcular_min(ITree nodo) {
  if (!itree_empty(nodo->left) && !itree_empty(nodo->right))
    return minimo(nodo->intv->izq, nodo->left->min);
  else if (!itree_empty(nodo->left) || !itree_empty(nodo->right)) {
    ITree nodoNoNULL = (!itree_empty(nodo->left)) ? nodo->left : nodo->right;
    return minimo(nodo->intv->izq, nodoNoNULL->min);
  } else
    return nodo->intv->izq;
}


/* Realiza una rotación a derecha del nodo dado como parámetro. */
ITree rotacion_a_derecha(ITree nodo) {
  ITree nuevaRaiz = nodo->left; // El hijo izquierdo del nodo pasará a ser la nueva raíz del árbol.
  ITree hijo_raiz = nuevaRaiz->right;
  nuevaRaiz->right = nodo;
  /* El hijo derecho del hijo izquierdo del nodo pasará a ser el hijo izquierdo
  del nodo sobre el cual se realizó la rotación. */
  nodo->left = hijo_raiz;
  /* Se actualizan las alturas, máximos y mínimos de los nodos rotados. */
  nodo->altura = 1 + maximo(itree_altura(nodo->right), itree_altura(nodo->left));
  nuevaRaiz->altura = 1 + maximo(itree_altura(nuevaRaiz->right), itree_altura(nuevaRaiz->left));
  nodo->max = calcular_max(nodo);
  nuevaRaiz->max = calcular_max(nuevaRaiz);
  nodo->min = calcular_min(nodo);
  nuevaRaiz->min = calcular_min(nuevaRaiz);
  return nuevaRaiz;
}


/* Realiza una rotación a izquierda del nodo dado como parámetro. */
ITree rotacion_a_izquierda(ITree nodo) {
  ITree nuevaRaiz = nodo->right; // El hijo derecho del nodo pasará a ser la nueva raíz del árbol.
  ITree hijo_raiz = nuevaRaiz->left;
  nuevaRaiz->left = nodo;
  /* El hijo izquierdo del hijo derecho del nodo pasará a ser el hijo derecho
  del nodo sobre el cual se realizó la rotación. */
  nodo->right = hijo_raiz;
  /* Se actualizan las alturas, máximos y mínimos de los nodos rotados. */
  nodo->altura = 1 + maximo(itree_altura(nodo->right), (itree_altura(nodo->left)));
  nuevaRaiz->altura = 1 + maximo(itree_altura(nuevaRaiz->right), (itree_altura(nuevaRaiz->left)));
  nodo->max = calcular_max(nodo);
  nuevaRaiz->max = calcular_max(nuevaRaiz);
  nodo->min = calcular_min(nodo);
  nuevaRaiz->min = calcular_min(nuevaRaiz);
  return nuevaRaiz;
}


/* Une dos intervalos que se intersecan en el nodo que guarda al primer intervalo. */
void union_ints(ITNodo* nodoIntv1, Intervalo* intv2) { 
  nodoIntv1->intv->izq = (nodoIntv1->intv->izq < intv2->izq) ? nodoIntv1->intv->izq : intv2->izq;
  nodoIntv1->intv->der = (nodoIntv1->intv->der > intv2->der) ? nodoIntv1->intv->der : intv2->der;
  nodoIntv1->max = calcular_max(nodoIntv1);
  nodoIntv1->min = calcular_min(nodoIntv1);
}


/* Busca en el nodo recursivamente nodos cuyos intervalos puedan unirse 
con aComparar, y los almacena en una pila pasada como parámetro. */
void buscar_repeticiones(ITree nodo, ITree aComparar, Pila* intvsAEliminar) {
  if (nodo) {
    if (intv_hay_interseccion(nodo->intv, aComparar->intv) 
      || ((long)(aComparar->intv->der))+1 == nodo->intv->izq
      || ((long)(aComparar->intv->izq))-1 == nodo->intv->der) {
      union_ints(aComparar, nodo->intv);
      *intvsAEliminar = pila_apilar(*intvsAEliminar, nodo->intv);
    }
    buscar_repeticiones(nodo->left, aComparar, intvsAEliminar);
    buscar_repeticiones(nodo->right, aComparar, intvsAEliminar);
  }
}

/* Une un intervalo al nodo en cuestión. Luego, busca en el resto del ITree 
posibles repeticiones del intervalo insertado, y las elimina de él.
Si direccion == 1, asume que intv puede unirse al extremo izquierdo del intervalo
del nodo. Caso contrario, asume que puede unirse al extremo derecho. */
ITree agregar_a_intv_existente(ITree nodo, Intervalo* intv, int direccion) {
  Pila intvsAEliminar = pila_crear();
  int otroExt = direccion ? nodo->intv->der : nodo->intv->izq;
  union_ints(nodo, intv);
  nodo->max = calcular_max(nodo);
  nodo->min = calcular_min(nodo);
  buscar_repeticiones((direccion) ? nodo->left : nodo->right, nodo, &intvsAEliminar);
  if (direccion ? intv->der > otroExt : intv->izq < otroExt)
    // En este caso, también puede haber repeticiones en el otro subárbol.
    buscar_repeticiones(direccion ? nodo->right : nodo->left, nodo, &intvsAEliminar);
  while (!pila_es_vacia(intvsAEliminar)) {
    nodo = itree_eliminar(nodo, pila_ultimo(intvsAEliminar));
    pila_desapilar(&intvsAEliminar);
  }
  pila_destruir(intvsAEliminar);
  return nodo;
}


/* Agrega un intervalo al ITree dado. */
ITree itree_insertar(ITree nodo, Intervalo *intv) {
  if (itree_empty(nodo)) { /* Si el nodo es vacío, se debe insertar aquí. */
    nodo = malloc(sizeof(ITNodo));
    nodo->intv = intv_crear();
    nodo->intv->izq = intv->izq;
    nodo->intv->der = intv->der;
    nodo->altura = 0;
    nodo->max = nodo->intv->der;
    nodo->min = nodo->intv->izq;
    nodo->left = NULL;
    nodo->right = NULL;
    return nodo;
  } else if (intv_comparar(intv, nodo->intv) < 0) { // El intervalo a insertar es menor que el nodo actual.
    /* Si puede agregarse al intervalo de un nodo existente, lo hace. */
    if ((long)(intv->der)+1 >= nodo->intv->izq) {
      nodo = agregar_a_intv_existente(nodo, intv, 1);
      return nodo;
    } else
      nodo->left = itree_insertar(nodo->left, intv);
  } else if (intv_comparar(intv, nodo->intv) > 0) { // El intervalo a insertar mayor que el nodo actual.
    /* Si puede agregarse al intervalo de un nodo existente, lo hace. */
    if ((long)(intv->izq)-1 <= nodo->intv->der) {
      nodo = agregar_a_intv_existente(nodo, intv, 0);
      return nodo;
    } else
    nodo->right = itree_insertar(nodo->right, intv);
  } else
    /* Si el intervalo a insertar está contenido en el nodo, no es necesario insertar nada en el ITree. */
    return nodo;
  /* El programa entrará en esta sección de la función solo si se llamó a sí misma
  recursivamente en el subárbol izquierdo o derecho. Acá se recalculan las alturas, máximos
  y mínimos de los nodos, y se balancea el árbol en caso de ser necesario. */
  nodo->altura = 1 + maximo(itree_altura(nodo->left), itree_altura(nodo->right));
  nodo->max = calcular_max(nodo);
  nodo->min = calcular_min(nodo);
  /* Se calcula el factor de balance del nodo, y en caso de desbalanceose realizan las rotaciones correspondientes. */
  int balance = itree_balance_factor(nodo);
  if (balance < -1) {
    /* Si el subárbol izquierdo es de menor altura que el derecho, y el nuevo elemento
    es lexicográficamente mayor al subárbol derecho, se debe realizar una rotación simple a izquierda. */
    if (intv_comparar(intv, nodo->right->intv) > 0)
      nodo = rotacion_a_izquierda(nodo);
    /* Caso contrario, se debe realizar una rotación doble derecha-izquierda. */
    else {
      nodo->right = rotacion_a_derecha(nodo->right);
      nodo = rotacion_a_izquierda(nodo);
    }
  } else if (balance > 1) {
    /* Si el subárbol derecho es de menor altura que el izquierdo, y el nuevo elemento
    es lexicográficamente menor al subárbol izquierdo, se debe realizar una rotación simple a derecha. */
    if (intv_comparar(intv, nodo->left->intv) < 0)
      nodo = rotacion_a_derecha(nodo);
    /* Caso contrario, se debe realizar una rotación doble izquierda-derecha. */
    else {
      nodo->left = rotacion_a_izquierda(nodo->left);
      nodo = rotacion_a_derecha(nodo);
    }
  }
  return nodo;
}


/* Elimina el intervalo pasado como parámetro si es exactamente igual 
al intervalo de algún nodo del ITree, y luego realiza las rotaciones 
correspondientes si el árbol resultante está desbalanceado. */
ITree itree_eliminar(ITree nodo, Intervalo *intv) {
  if (!itree_empty(nodo)) {
    if (nodo->intv->izq == intv->izq && nodo->intv->der == intv->der) {
      /* En este caso, el nodo actual es el nodo a eliminar. Se distinguen los casos 
      en los que el nodo tiene ninguno, uno o dos hijos no NULL. Los dos primeros
      son triviales, y en el caso donde tiene dos hijos no NULL, el nodo a eliminar 
      se reemplaza por el mínimo nodo del subárbol derecho. */
      if (itree_empty(nodo->left) && itree_empty(nodo->right)) {
        intv_destruir(nodo->intv);
        free(nodo);
        nodo = NULL;
      } else if (!itree_empty(nodo->left) && !itree_empty(nodo->right)) {
        ITree minNodoDerecho = nodo->right;
        while (!itree_empty(minNodoDerecho->left))
          minNodoDerecho = minNodoDerecho->left;
        nodo->intv->izq = minNodoDerecho->intv->izq;
        nodo->intv->der = minNodoDerecho->intv->der;
        nodo->right = itree_eliminar(nodo->right, minNodoDerecho->intv);
      } else {
        ITree hijoNoNULL = (!itree_empty(nodo->left)) ? nodo->left : nodo->right;
        intv_destruir(nodo->intv);
        free(nodo);
        nodo = hijoNoNULL;
      }
    } else {
      /* Si el nodo actual no es el nodo a eliminar, se sigue recorriendo el ITree. */
      nodo->left = itree_eliminar(nodo->left, intv);
      nodo->right = itree_eliminar(nodo->right, intv);
    }
  }
  if (itree_empty(nodo))
    return nodo;
  /* Se actualizan la altura, el máximo y el mínimo del nodo. */
  nodo->altura = 1 + maximo(itree_altura(nodo->left),itree_altura(nodo->right));
  nodo->max = calcular_max(nodo);
  nodo->min = calcular_min(nodo);
  /* Se calcula el factor de balance del nodo, y en caso de desbalanceo se realizan las rotaciones correspondientes. */
  int balance = itree_balance_factor(nodo);
  if (balance < -1) {
    /* Si el subárbol izquierdo es de menor altura que el derecho (y está desbalanceado), y lo mismo sucede
    para los subárboles del subárbol derecho, se debe realizar una rotación simple a izquierda. */
    if (itree_balance_factor(nodo->right) < 0)
      nodo = rotacion_a_izquierda(nodo);
    /* En otro caso, se debe realizar una rotación doble derecha-izquierda. */
    else {
      nodo->right = rotacion_a_derecha(nodo->right);
      nodo = rotacion_a_izquierda(nodo);
    }
  } else if (balance > 1) {
    /* Si el subárbol derecho es de menor altura que el izquierdo (y está desbalanceado), y lo mismo sucede
    para los subárboles del subárbol izquierdo, se debe realizar una rotación simple a derecha. */
    if (itree_balance_factor(nodo->left) > 0)
      nodo = rotacion_a_derecha(nodo);
    /* En otro caso, se debe realizar una rotación doble izquierda-derecha. */
    else {
      nodo->left = rotacion_a_izquierda(nodo->left);
      nodo = rotacion_a_derecha(nodo);
    }
  }
  return nodo;
}
