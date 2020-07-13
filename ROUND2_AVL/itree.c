#include <stdlib.h>
#include <stdio.h>
#include "itree.h"
#include "cola.h"

/* Archivo con la implementación de Árboles de Intervalos AVL. */


ITree itree_crear() {
  return NULL;
}


void itree_destruir(ITree nodo) {
  if (!itree_empty(nodo)) {
    itree_destruir(nodo->left);
    itree_destruir(nodo->right);
    free(nodo->intv);
    free(nodo);
  }
}


int itree_empty(ITree nodo) {
  return nodo == NULL;
}


int itree_altura(ITree nodo) {
  return (itree_empty(nodo)) ? -1 : nodo->altura;
}


int itree_balance_factor(ITree nodo) {
  return itree_altura(nodo->left) - itree_altura(nodo->right);
}


/* Devuelve el máximo de dos doubles. */
double maximo(double n1, double n2) {
  return n1 < n2 ? n2 : n1;
}


/* Calcula la altura de un ITree luego de insertar un nodo. */
int calcular_altura(ITree nodo) {
  return 1 + (int) maximo(itree_altura(nodo->right), itree_altura(nodo->left));
}


/* Calcula el máximo extremo derecho de un ITree luego de insertar un nodo. */
double calcular_max(ITree nodo) {
  if (!itree_empty(nodo->left) && !itree_empty(nodo->right))
    return maximo(maximo(nodo->intv->der, nodo->left->max), nodo->right->max);
  else if (!itree_empty(nodo->left) || !itree_empty(nodo->right)) {
    ITree nodoNoNULL = (!itree_empty(nodo->left)) ? nodo->left : nodo->right;
    return maximo(nodo->intv->der, nodoNoNULL->max);
  } else
    return nodo->intv->der;
}


double comparar_intervalos(Intervalo *intv1, Intervalo *intv2) {
  return (intv1->izq == intv2->izq) ? intv1->der - intv2->der : intv1->izq - intv2->izq;
}


/* Realiza una rotación a derecha del nodo dado como parámetro. */
ITree rotacion_a_derecha(ITree nodo) {
  ITree nuevaRaiz = nodo->left; // El hijo izquierdo del nodo pasará a ser la nueva raíz del árbol.
  ITree hijo_raiz = nuevaRaiz->right;
  nuevaRaiz->right = nodo;
  /* El hijo derecho del hijo izquierdo del nodo pasará a ser el hijo izquierdo
  del nodo sobre el cual se realizó la rotación. */
  nodo->left = hijo_raiz;
  /* Se actualizan las alturas y máximos de los nodos rotados. */
  nodo->altura = 1 + (int) maximo(itree_altura(nodo->right), itree_altura(nodo->left));
  nuevaRaiz->altura = 1 + (int) maximo(itree_altura(nuevaRaiz->right), itree_altura(nuevaRaiz->left));
  nodo->max = calcular_max(nodo);
  nuevaRaiz->max = calcular_max(nuevaRaiz);
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
  /* Se actualizan las alturas y máximos de los nodos rotados. */
  nodo->altura = 1 + (int) maximo(itree_altura(nodo->right), (itree_altura(nodo->left)));
  nuevaRaiz->altura = 1 + (int) maximo(itree_altura(nuevaRaiz->right), (itree_altura(nuevaRaiz->left)));
  nodo->max = calcular_max(nodo);
  nuevaRaiz->max = calcular_max(nuevaRaiz);
  return nuevaRaiz;
}


/* itree_insertar inserta un nodo en el lugar correspondiente del ITree, y luego
realiza las rotaciones adecuadas si el árbol resultante está desbalanceado. */
ITree itree_insertar(ITree nodo, Intervalo *intv) {
  if (itree_empty(nodo)) { /* Si el nodo es vacío, se debe insertar aquí. */
    nodo = malloc(sizeof(ITNodo));
    nodo->intv = malloc(sizeof(Intervalo));
    nodo->intv->izq = intv->izq;
    nodo->intv->der = intv->der;
    nodo->altura = 0;
    nodo->max = nodo->intv->der;
    nodo->left = NULL;
    nodo->right = NULL;
    return nodo;
    } else if (comparar_intervalos(intv, nodo->intv) < 0) {
    /* Si el intervalo a insertar es menor según el orden lexicográfico, se insertará en el subárbol izquierdo. */
    nodo->left = itree_insertar(nodo->left, intv);
    } else if (comparar_intervalos(intv, nodo->intv) > 0) {
    /* Si el intervalo a insertar es mayor según el orden lexicográfico, se insertará en el subárbol derecho. */
    nodo->right = itree_insertar(nodo->right, intv);
  } else {
    /* Si el intervalo a insertar ya está en el árbol, no se inserta nada. */
    return nodo;
  }
  /* El programa entrará en esta sección de la función solo si se llamó a sí misma
  recursivamente en el subárbol izquierdo o derecho. Si el intervalo no está presente en el árbol, 
  la recursión desciende hasta alguna hoja del mismo donde se insertará el elemento, y luego 
  las alturas y máximos se irán actualizando desde abajo hacia arriba. */
  nodo->altura = 1 + (int) maximo((double) itree_altura(nodo->left), (double) itree_altura(nodo->right));
  nodo->max = calcular_max(nodo);
  /* Se calcula el factor balance del nodo, y en caso de desbalanceo se realizan las rotaciones correspondientes. */
  int balance = itree_balance_factor(nodo);
  if (balance < -1) {
    /* Si el subárbol izquierdo es de menor altura que el derecho, y el nuevo elemento
    es lexicográficamente mayor al subárbol derecho, se debe realizar una rotación simple a izquierda. */
    if (comparar_intervalos(intv, nodo->right->intv) > 0)
      nodo = rotacion_a_izquierda(nodo);
    /* Caso contrario, se debe realizar una rotación doble derecha-izquierda. */
    else {
      nodo->right = rotacion_a_derecha(nodo->right);
      nodo = rotacion_a_izquierda(nodo);
    }
  } else if (balance > 1) {
    /* Si el subárbol derecho es de menor altura que el izquierdo, y el nuevo elemento
    es lexicográficamente menor al subárbol izquierdo, se debe realizar una rotación simple a derecha. */
    if (comparar_intervalos(intv, nodo->left->intv) < 0)
      nodo = rotacion_a_derecha(nodo);
    /* Caso contrario, se debe realizar una rotación doble izquierda-derecha. */
    else {
      nodo->left = rotacion_a_izquierda(nodo->left);
      nodo = rotacion_a_derecha(nodo);
    }
  }
  return nodo;
}


/* itree_eliminar elimina un nodo si está presente en el ITree, y luego
realiza las rotaciones correspondientes si el árbol resultante está desbalanceado. */
ITree itree_eliminar(ITree nodo, Intervalo *intv) {
  if (!itree_empty(nodo)) {
    if (comparar_intervalos(intv, nodo->intv) == 0) { // Si el nodo actual es el nodo a eliminar.
      /* A continuación se distinguen los casos en los que el nodo tiene ninguno, uno o dos hijos no NULL.
      Los dos primeros casos son triviales, y en el caso donde tiene dos hijos no NULL,
      el nodo a eliminar se reemplaza por el mínimo nodo del subárbol derecho. */
      if (itree_empty(nodo->left) && itree_empty(nodo->right)) {
        free(nodo->intv);
        free(nodo);
        nodo = NULL;
      } else if (!itree_empty(nodo->left) && !itree_empty(nodo->right)) {
        ITree minNodoDerecho = nodo->right;
        while (!itree_empty(minNodoDerecho->left)) {
          minNodoDerecho = minNodoDerecho->left;
        }
        nodo->intv->izq = minNodoDerecho->intv->izq;
        nodo->intv->der = minNodoDerecho->intv->der;
        nodo->right = itree_eliminar(nodo->right, minNodoDerecho->intv);
      } else {
        ITree hijoNoNULL = (!itree_empty(nodo->left)) ? nodo->left : nodo->right;
        free(nodo->intv);
        free(nodo);
        nodo = hijoNoNULL;
      }
       } else if (comparar_intervalos(intv, nodo->intv) < 0) {
      /* Si el intervalo a eliminar es menor según el orden lexicográfico, si está
      en el árbol estará en el subárbol izquierdo, por lo que se llama a la función
      recursivamente sobre el subárbol izquierdo. */
      nodo->left = itree_eliminar(nodo->left, intv);
    } else {
      /* En caso contrario, si el intervalo a eliminar es un nodo del árbol, 
      estará en el subárbol derecho. */
      nodo->right = itree_eliminar(nodo->right, intv);
    }
  }
  if (itree_empty(nodo))
    return nodo;
  /* Se actualizan la altura y el máximo del árbol. */
  nodo->altura = 1 + (int) maximo((double) itree_altura(nodo->left),(double) itree_altura(nodo->right));
  nodo->max = calcular_max(nodo);
  /* Se calcula el factor balance del árbol, y en caso de desbalanceo se realizan las rotaciones correspondientes. */
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


ITree itree_intersecar(ITree arbol, Intervalo *intv) {
  ITree interseccion = NULL;
  if (!itree_empty(arbol)) { 
    if (intv->der < arbol->intv->izq || intv->izq > arbol->intv->der) { /* El intervalo no se interseca con la raíz. */
      /* Si su subárbol izquierdo es no vacío y si el máximo del subárbol izquierdo es mayor o igual 
      al extremo izquierdo del intervalo, entonces es seguro que habrá intersección en ese subárbol. (*) */
      if (!itree_empty(arbol->left) && intv->izq <= arbol->left->max)
        interseccion = itree_intersecar(arbol->left, intv);
      /* (Si intv->izq es mayor que el ext. derecho de la raíz) Si su subárbol derecho es no vacío y si el máximo 
      del subárbol derecho es mayor o igual a intv->izq, entonces es posible que haya intersección en ese subárbol. */
      else if (intv->izq > arbol->intv->der && !itree_empty(arbol->right) && intv->izq <= arbol->right->max)
        interseccion = itree_intersecar(arbol->right, intv);
    } else // En caso contrario, se interseca con el intervalo de la raíz.
        interseccion = arbol;
  }
  return interseccion;
}


void imprimir_intervalo(ITree nodo) {
  if (!itree_empty(nodo)) printf("[%g, %g] ", nodo->intv->izq, nodo->intv->der);
}


void itree_recorrer_dfs(ITree arbol, FuncionVisitante visit) {
  if (!itree_empty(arbol)) {
    itree_recorrer_dfs(arbol->left, visit);
    visit(arbol);
    itree_recorrer_dfs(arbol->right, visit);
  }
}


/* La función itree_recorrer_bfs utiliza colas como estructura auxiliar
para almacenar los nodos a visitar. */
void itree_recorrer_bfs(ITree arbol, FuncionVisitante visit) {
  if (!itree_empty(arbol)) {
    /* Crea una cola con la raiz del árbol. */
    Cola queue = cola_crear();
    cola_encolar(queue, arbol);
    ITNodo *temp;
    while (!cola_es_vacia(queue)) {
      /* Mientras la cola no sea vacía, visito el primer elemento y lo elimino de la cola. */
      temp = cola_primero(queue);
      cola_desencolar(queue);
      visit(temp);
      /* Luego agrego a la cola los hijos de ese nodo, en el orden que quiero que sean visitados. */
      if (!itree_empty(temp->left)) cola_encolar(queue, temp->left);
      if (!itree_empty(temp->right)) cola_encolar(queue, temp->right);
    }
    cola_destruir(queue);
  }
}
