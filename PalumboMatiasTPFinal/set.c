#include "set.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

/* Archivo con la implementación de conjuntos (Sets). */


Set set_crear() {
  return itree_crear();
}


void set_destruir(Set nodo) {
  itree_destruir(nodo);
}


int set_empty(Set nodo) {
  return itree_empty(nodo);
}


/* Agrega un intervalo al Set. */
Set set_insertar(Set set, Intervalo *intv) {
  return itree_insertar(set, intv);
}


/* Función auxiliar para imprimir recursivamente un Set. */
void set_imprimir_aux(Set nodo, int max) {
  if (!set_empty(nodo)) {
    set_imprimir_aux(nodo->left, max);
    intv_imprimir(nodo->intv);
    /* Si el extremo derecho es igual al máximo, es el último nodo del conjunto,
    ya que todos los intervalos son disjuntos dos a dos. */
    if (nodo->intv->der != max)
      printf(",");
    set_imprimir_aux(nodo->right, max);
  }
}

void set_imprimir(Set set) {
  if (!set_empty(set))
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


/* Agrega recursivamente los nodos de menorAltura a mayorAltura. */
Set generar_union(Set resultado, Set menorAltura) {
  if (set_empty(menorAltura))
    return resultado;
  resultado = set_insertar(resultado, menorAltura->intv);
  resultado = generar_union(resultado, menorAltura->left);
  resultado = generar_union(resultado, menorAltura->right);
  return resultado;
}

Set set_union(Set set1, Set set2) {
  if (set_empty(set1) || set_empty(set2)) {
    Set resultado = set_clonar((set_empty(set1)) ? set2 : set1);
    return resultado;
  }
  /* Se distingue el conjunto cuyo árbol es de mayor altura con el de menor altura. */
  Set menorAltura = (set2->altura < set1->altura) ? set2 : set1;
  Set mayorAltura = (set2->altura < set1->altura) ? set1 : set2;
  /* Se crea una copia del Set de mayor altura, y luego se agregar los elementos de menorAltura a él. */
  Set resultado = set_clonar(mayorAltura);
  resultado = generar_union(resultado, menorAltura);
  return resultado;
}


/* Función auxiliar en la generación del complemento de un Set. */
Set generar_complemento(Set resultado, Set nodo, Intervalo* intv) {
  if (set_empty(nodo))
    return resultado;
  /* En ambos condicionales, si existen enteros entre los intervalos 
  de dos nodos consecutivos (según el recorrido in-order), 
  se agregan al complemento, y se continúa recorriendo el Set. */
  if (nodo->left) {
    intv->izq = nodo->left->max+1;
    intv->der = nodo->intv->izq-1;
    resultado = set_insertar(resultado, intv);
    resultado = generar_complemento(resultado, nodo->left, intv);
  }
  if (nodo->right) {
    intv->izq = nodo->intv->der+1;
    intv->der = nodo->right->min-1;
    resultado = set_insertar(resultado, intv);
    resultado = generar_complemento(resultado, nodo->right, intv);
  }
  return resultado;
}

Set set_complemento(Set set) {
  Set resultado = set_crear();
  Intervalo* intv = intv_crear();
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
  intv_destruir(intv);
  return resultado;
}


/* Busca y agrega a resultado todas las intersecciones de intv con los elementos de set. */
Set set_intersecar(Set set, Intervalo *intv, Intervalo* buffer, Set resultado) {
  if (set_empty(set))
    return resultado;
  if (intv_hay_interseccion(set->intv, intv)) {
    buffer->izq = (set->intv->izq > intv->izq) ? set->intv->izq : intv->izq;
    buffer->der = (set->intv->der < intv->der) ? set->intv->der : intv->der;
    resultado = set_insertar(resultado, buffer);
  }
  /* Verifica si es posible que haya intersección en los subárboles del nodo del conjunto,
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
  /* Se distingue el conjunto con árbol de mayor altura del de menor altura. */
  Set menorAltura = (set2->altura < set1->altura) ? set2 : set1;
  Set mayorAltura = (set2->altura < set1->altura) ? set1 : set2;
  Intervalo* intv = intv_crear();
  Set resultado = set_crear();
  resultado = generar_interseccion(resultado, mayorAltura, menorAltura, intv);
  intv_destruir(intv);
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
