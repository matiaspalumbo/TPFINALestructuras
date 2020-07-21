#include <stdlib.h>
#include <stdio.h>
#include "set.h"
#include "pila.h"
#include <limits.h>

/* Archivo con la implementación de conjuntos (Sets). */


Set set_crear() {
  return NULL;
}


void set_destruir(Set nodo) {
  if (!set_empty(nodo)) {
    set_destruir(nodo->left);
    set_destruir(nodo->right);
    free(nodo->intv);
    free(nodo);
  }
}


int set_empty(Set nodo) {
  return nodo == NULL;
}


/* Devuelve la altura del nodo. */
int itree_altura(Set nodo) {
  return (set_empty(nodo)) ? -1 : nodo->altura;
}


/* Devuelve el factor de balance del nodo. */
int itree_balance_factor(Set nodo) {
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


/* Calcula el máximo extremo derecho de un Set luego de insertar un nodo. */
int calcular_max(Set nodo) {
  if (!set_empty(nodo->left) && !set_empty(nodo->right))
    return maximo(maximo(nodo->intv->der, nodo->left->max), nodo->right->max);
  else if (!set_empty(nodo->left) || !set_empty(nodo->right)) {
    Set nodoNoNULL = (!set_empty(nodo->left)) ? nodo->left : nodo->right;
    return maximo(nodo->intv->der, nodoNoNULL->max);
  } else
    return nodo->intv->der;
}


/* Calcula el mínimo extremo izquierdo de un Set luego de insertar un nodo. */
int calcular_min(Set nodo) {
  if (!set_empty(nodo->left) && !set_empty(nodo->right))
    return minimo(nodo->intv->izq, nodo->left->min);
  else if (!set_empty(nodo->left) || !set_empty(nodo->right)) {
    Set nodoNoNULL = (!set_empty(nodo->left)) ? nodo->left : nodo->right;
    return minimo(nodo->intv->izq, nodoNoNULL->min);
  } else
    return nodo->intv->izq;
}


/* Devuelve 0 si intvA está contenido en intvB. Caso contrario,
los compara de acuerdo al orden lexicográfico. */
long comparar_intervalos(Intervalo *intvA, Intervalo *intvB) {
  if (intvA->izq >= intvB->izq && intvA->der <= intvB->der)
    return 0;
  else
    return  (intvA->izq == intvB->izq) ? ((long)intvA->der - intvB->der) : ((long)intvA->izq - intvB->izq);
}


/* Realiza una rotación a derecha del nodo dado como parámetro. */
Set rotacion_a_derecha(Set nodo) {
  Set nuevaRaiz = nodo->left; // El hijo izquierdo del nodo pasará a ser la nueva raíz del árbol.
  Set hijo_raiz = nuevaRaiz->right;
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
Set rotacion_a_izquierda(Set nodo) {
  Set nuevaRaiz = nodo->right; // El hijo derecho del nodo pasará a ser la nueva raíz del árbol.
  Set hijo_raiz = nuevaRaiz->left;
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
void union_ints(SetNodo* nodoIntv1, Intervalo* intv2) { 
  nodoIntv1->intv->izq = (nodoIntv1->intv->izq < intv2->izq) ? nodoIntv1->intv->izq : intv2->izq;
  nodoIntv1->intv->der = (nodoIntv1->intv->der > intv2->der) ? nodoIntv1->intv->der : intv2->der;
  nodoIntv1->max = calcular_max(nodoIntv1);
  nodoIntv1->min = calcular_min(nodoIntv1);
}


/* Verifica si dos intervalos se intersecan. */
int hay_interseccion(Intervalo* intv1, Intervalo* intv2) {
  return intv1->der >= intv2->izq && intv1->izq <= intv2->der;
}


/* Busca en el nodo recursivamente nodos cuyos intervalos puedan unirse 
con aComparar, y los almacena en una pila pasada como parámetro. */
void buscar_repeticiones(Set nodo, Set aComparar, Pila* intvsAEliminar) {
  if (nodo) {
    if (hay_interseccion(nodo->intv, aComparar->intv) 
      || ((long)(aComparar->intv->der))+1 == nodo->intv->izq
      || ((long)(aComparar->intv->izq))-1 == nodo->intv->der) {
      union_ints(aComparar, nodo->intv);
      *intvsAEliminar = pila_apilar(*intvsAEliminar, nodo->intv);
    }
    buscar_repeticiones(nodo->left, aComparar, intvsAEliminar);
    buscar_repeticiones(nodo->right, aComparar, intvsAEliminar);
  }
}

/* Une un intervalo al nodo en cuestión. Luego, busca en el resto del Set 
posibles repeticiones del intervalo insertado, y las elimina de él.
Si direccion == 1, asume que intv puede unirse al extremo izquierdo del intervalo
del nodo. Caso contrario, asume que puede unirse al extremo derecho. */
Set agregar_a_intv_existente(Set nodo, Intervalo* intv, int direccion) {
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
    nodo = set_eliminar(nodo, pila_ultimo(intvsAEliminar));
    pila_desapilar(&intvsAEliminar);
  }
  pila_destruir(intvsAEliminar);
  return nodo;
}


/* Agrega un intervalo al Set. */
Set set_insertar(Set nodo, Intervalo *intv) {
  if (set_empty(nodo)) { /* Si el nodo es vacío, se debe insertar aquí. */
    nodo = malloc(sizeof(SetNodo));
    nodo->intv = malloc(sizeof(Intervalo));
    nodo->intv->izq = intv->izq;
    nodo->intv->der = intv->der;
    nodo->altura = 0;
    nodo->max = nodo->intv->der;
    nodo->min = nodo->intv->izq;
    nodo->left = NULL;
    nodo->right = NULL;
    return nodo;
  } else if (comparar_intervalos(intv, nodo->intv) < 0) { // El intervalo a insertar es menor que el nodo actual.
    /* Si puede agregarse al intervalo de un nodo existente, lo hace. */
    if ((long)(intv->der)+1 >= nodo->intv->izq) {
      nodo = agregar_a_intv_existente(nodo, intv, 1);
      return nodo;
    } else
      nodo->left = set_insertar(nodo->left, intv);
  } else if (comparar_intervalos(intv, nodo->intv) > 0) { // El intervalo a insertar mayor que el nodo actual.
    /* Si puede agregarse al intervalo de un nodo existente, lo hace. */
    if ((long)(intv->izq)-1 <= nodo->intv->der) {
      nodo = agregar_a_intv_existente(nodo, intv, 0);
      return nodo;
    } else
    nodo->right = set_insertar(nodo->right, intv);
  } else
    /* Si el intervalo a insertar está contenido en el nodo, no es necesario insertar nada en el Set. */
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


/* Elimina el intervalo pasado como parámetro si es exactamente igual 
al intervalo de algún nodo del Set, y luego realiza las rotaciones 
correspondientes si el árbol resultante está desbalanceado. */
Set set_eliminar(Set nodo, Intervalo *intv) {
  if (!set_empty(nodo)) {
    if (nodo->intv->izq == intv->izq && nodo->intv->der == intv->der) {
      /* En este caso, el nodo actual es el nodo a eliminar. Se distinguen los casos 
      en los que el nodo tiene ninguno, uno o dos hijos no NULL. Los dos primeros
      son triviales, y en el caso donde tiene dos hijos no NULL, el nodo a eliminar 
      se reemplaza por el mínimo nodo del subárbol derecho. */
      if (set_empty(nodo->left) && set_empty(nodo->right)) {
        free(nodo->intv);
        free(nodo);
        nodo = NULL;
      } else if (!set_empty(nodo->left) && !set_empty(nodo->right)) {
        Set minNodoDerecho = nodo->right;
        while (!set_empty(minNodoDerecho->left))
          minNodoDerecho = minNodoDerecho->left;
        nodo->intv->izq = minNodoDerecho->intv->izq;
        nodo->intv->der = minNodoDerecho->intv->der;
        nodo->right = set_eliminar(nodo->right, minNodoDerecho->intv);
      } else {
        Set hijoNoNULL = (!set_empty(nodo->left)) ? nodo->left : nodo->right;
        free(nodo->intv);
        free(nodo);
        nodo = hijoNoNULL;
      }
    } else {
      /* Si el nodo actual no es el nodo a eliminar, se sigue recorriendo el Set. */
      nodo->left = set_eliminar(nodo->left, intv);
      nodo->right = set_eliminar(nodo->right, intv);
    }
  }
  if (set_empty(nodo))
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


/* Imprime un intervalo. Si los extremos son iguales, imprime 
solo un extremo, y si no, imprime ambos separados por ":". */
void imprimir_intv(void* intv) {
  if (((Intervalo*)intv)->izq == ((Intervalo*)intv)->der)
    printf("%d", ((Intervalo*)intv)->izq);
  else
    printf("%d:%d", ((Intervalo*)intv)->izq, ((Intervalo*)intv)->der);
}

/* Función auxiliar para imprimir recursivamente un Set. */
void set_imprimir_aux(Set set, int max) {
  if (set) {
    set_imprimir_aux(set->left, max);
    imprimir_intv(set->intv);
    /* Si el extremo derecho es igual al máximo, es el último nodo del árbol,
    ya que los intervalos del árbol son disjuntos dos a dos. */
    if (set->intv->der != max)
      printf(",");
    set_imprimir_aux(set->right, max);
  }
}

void set_imprimir(Set set) {
  if (set)
    set_imprimir_aux(set, set->max);
  else
    printf("{}");
}


/* Función auxiliar para clonar recursivamente un Set. */
Set set_clonar_aux(Set resultado, Set nodo) {
  if (set_empty(nodo))
    return resultado;
  resultado = set_insertar(resultado, nodo->intv);
  resultado = set_clonar_aux(resultado, nodo->left);
  resultado = set_clonar_aux(resultado, nodo->right);
  return resultado;
}

Set set_clonar(Set set) {
    if (set_empty(set))
        return set_crear();
    Set resultado = set_crear();
    resultado = set_clonar_aux(resultado, set);
    return resultado;
}

/* Agrega recursivamente los nodos de menorAltura en mayorAltura. */
Set unir_sets(Set resultado, Set menorAltura) {
  if (set_empty(menorAltura))
    return resultado;
  resultado = set_insertar(resultado, menorAltura->intv);
  resultado = unir_sets(resultado, menorAltura->left);
  resultado = unir_sets(resultado, menorAltura->right);
  return resultado;
}

Set set_union(Set set1, Set set2) {
  if (set_empty(set1) || set_empty(set2)) {
    Set resultado = set_clonar((set_empty(set1)) ? set2 : set1);
    return resultado;
  }
  /* Se distingue el árbol de mayor altura del menor altura. */
  Set menorAltura = (set2->altura < set1->altura) ? set2 : set1;
  Set mayorAltura = (set2->altura < set1->altura) ? set1 : set2;
  /* Se crea una copia del Set de mayor altura, y luego se agregar los elementos de menorAltura a él. */
  Set resultado = set_clonar(mayorAltura);
  resultado = unir_sets(resultado, menorAltura);
  return resultado;
}


/* Función auxiliar en la generación del complemento de un Set. */
Set generar_complemento(Set resultado, Set nodo, Intervalo* intv) {
  if (set_empty(nodo))
    return resultado;
  /* En ambos condicionales, si existen enteros entre los intervalos 
  de dos nodos consecutivos (según el recorrido in-order), 
  se agregan al complemento, y se continúa recorriendo el Set. */
  if (nodo->left && nodo->left->max+1 < nodo->intv->izq) {
    intv->izq = nodo->left->max+1;
    intv->der = nodo->intv->izq-1;
    resultado = set_insertar(resultado, intv);
    resultado = generar_complemento(resultado, nodo->left, intv);
  }
  if (nodo->right && nodo->right->min-1 > nodo->intv->der) {
    intv->izq = nodo->intv->der+1;
    intv->der = nodo->right->min-1;
    resultado = set_insertar(resultado, intv);
    resultado = generar_complemento(resultado, nodo->right, intv);
  }
  return resultado;
}

Set set_complemento(Set set) {
  Set resultado = set_crear();
  Intervalo* intv = malloc(sizeof(Intervalo));
  if (set_empty(set)) { // Si el Set es vacío, el complemento es todo el universo.
    intv->izq = INT_MIN;
    intv->der = INT_MAX;
    resultado = set_insertar(resultado, intv);
  } else {
    /* Si los extremos mínimos y máximos del Set son distintos de los extremos 
    del universo, se agregan al complemento los intervalos correspondientes. */
    if (set->min != INT_MIN) {
      intv->izq = INT_MIN;
      intv->der = set->min - 1;
      resultado = set_insertar(resultado, intv);
    }
    if (set->max != INT_MAX) {
      intv->izq = set->max + 1;
      intv->der = INT_MAX;
      resultado = set_insertar(resultado, intv);
    }
    resultado = generar_complemento(resultado, set, intv);
  }
  free(intv);
  return resultado;
}


/* Busca y agrega a resultado todas las intersecciones de intv con los elementos de set. */
Set set_intersecar(Set set, Intervalo *intv, Intervalo* buffer, Set resultado) {
  if (set_empty(set))
    return resultado;
  if (hay_interseccion(set->intv, intv)) {
    buffer->izq = (set->intv->izq > intv->izq) ? set->intv->izq : intv->izq;
    buffer->der = (set->intv->der < intv->der) ? set->intv->der : intv->der;
    resultado = set_insertar(resultado, buffer);
  }
  /* Verifica si es posible que haya intersección en los subárboles,
  y, en caso de serlo, continúa la búsqueda. */
  if (!set_empty(set->left) && intv->izq <= set->left->max && intv->der >= set->left->min)
    resultado = set_intersecar(set->left, intv, buffer, resultado);
  if (!set_empty(set->right) && intv->izq <= set->right->max && intv->der >= set->right->min)
    resultado = set_intersecar(set->right, intv, buffer, resultado);
  return resultado;
}

/* Función auxiliar que interseca dos sets recursivamente. */
Set generar_interseccion(Set resultado, Set mayorAltura, Set menorAltura, Intervalo* intv) {
  if (set_empty(menorAltura))
    return resultado;
  resultado = set_intersecar(mayorAltura, menorAltura->intv, intv, resultado);
  resultado = generar_interseccion(resultado, mayorAltura, menorAltura->left, intv);
  resultado = generar_interseccion(resultado, mayorAltura, menorAltura->right, intv);
  return resultado;
}

Set set_interseccion(Set set1, Set set2) {
  if (set_empty(set1) || set_empty(set2))
    return NULL;
  /* Se distingue el árbol de mayor altura del menor altura. */
  Set menorAltura = (set2->altura < set1->altura) ? set2 : set1;
  Set mayorAltura = (set2->altura < set1->altura) ? set1 : set2;
  Intervalo* intv = malloc(sizeof(Intervalo));
  Set resultado = set_crear();
  resultado = generar_interseccion(resultado, mayorAltura, menorAltura, intv);
  free(intv);
  return resultado;
}


/* Teniendo en cuenta que la diferencia de A con B es matemáticamente igual a la
intersección de A con el complemento de B, set_resta devuelve el resultado de 
restar set2 a set1.  */
Set set_resta(Set set1, Set set2) {
  Set resultado = set_crear();
  Set complemento = set_crear();
  complemento = set_complemento(set2);
  resultado = set_interseccion(set1, complemento);
  set_destruir(complemento);
  return resultado;
}
