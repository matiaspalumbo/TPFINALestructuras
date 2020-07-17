#include <stdlib.h>
#include <stdio.h>
#include "set.h"
#include "pila.h"
#include "limits.h"

/* Archivo con la implementación de Árboles de Intervalos AVL. */

void imprimir_intv(void* intv) {
  if (((Intervalo*)intv)->izq == ((Intervalo*)intv)->der) {
    printf("%d", ((Intervalo*)intv)->izq);
  } else {
    printf("%d:%d", ((Intervalo*)intv)->izq, ((Intervalo*)intv)->der);
  }
}

void printggg(void* intv) {
  printf("%d:%d,", ((Intervalo*)intv)->izq, ((Intervalo*)intv)->der);
}

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


int itree_altura(Set nodo) {
  return (set_empty(nodo)) ? -1 : nodo->altura;
}


int itree_balance_factor(Set nodo) {
  return itree_altura(nodo->left) - itree_altura(nodo->right);
}


int maximo(int n1, int n2) {
  return n1 < n2 ? n2 : n1;
}

int minimo(int n1, int n2) {
  return n1 < n2 ? n1 : n2;
}

/* Calcula la altura de un Set luego de insertar un nodo. */
int calcular_altura(Set nodo) {
  return 1 + maximo(itree_altura(nodo->right), itree_altura(nodo->left));
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


long comparar_intervalos(Intervalo *intvA, Intervalo *intvB) {
  return (intvA->izq >= intvB->izq && intvA->der <= intvB->der) ? 0 : (long)intvA->izq - (long)intvB->izq;
  // return (intv1->izq == intv2->izq) ? (long)intv1->der - (long)intv2->der : (long)intv1->izq - (long)intv2->izq;
  // return (intv1->izq == intv2->izq) ? intv1->der - intv2->der : intv1->izq - intv2->izq;
  // return (long)intv1->izq - (long)intv2->izq;
}

// Devuelve 1 si B esta contenido en A, 0 en otro caso;
// int esta_contenido(Intervalo* intvA, Intervalo* intvB) {
  // return intvB->izq >= intvA->izq && intvB->der <= intvA->der;
// }

/* Realiza una rotación a derecha del nodo dado como parámetro. */
Set rotacion_a_derecha(Set nodo) {
  Set nuevaRaiz = nodo->left; // El hijo izquierdo del nodo pasará a ser la nueva raíz del árbol.
  Set hijo_raiz = nuevaRaiz->right;
  nuevaRaiz->right = nodo;
  /* El hijo derecho del hijo izquierdo del nodo pasará a ser el hijo izquierdo
  del nodo sobre el cual se realizó la rotación. */
  nodo->left = hijo_raiz;
  /* Se actualizan las alturas y máximos de los nodos rotados. */
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
  /* Se actualizan las alturas y máximos de los nodos rotados. */
  nodo->altura = 1 + maximo(itree_altura(nodo->right), (itree_altura(nodo->left)));
  nuevaRaiz->altura = 1 + maximo(itree_altura(nuevaRaiz->right), (itree_altura(nuevaRaiz->left)));
  nodo->max = calcular_max(nodo);
  nuevaRaiz->max = calcular_max(nuevaRaiz);
  nodo->min = calcular_min(nodo);
  nuevaRaiz->min = calcular_min(nuevaRaiz);
  return nuevaRaiz;
}

void union_ints(SetNodo* nodoIntv1, Intervalo* intv2) { // Une dos intervalos que se intersecan.
  // Intervalo* intv = malloc(sizeof(Intervalo));
  nodoIntv1->intv->izq = (nodoIntv1->intv->izq < intv2->izq) ? nodoIntv1->intv->izq : intv2->izq;
  nodoIntv1->intv->der = (nodoIntv1->intv->der > intv2->der) ? nodoIntv1->intv->der : intv2->der;
  nodoIntv1->max = calcular_max(nodoIntv1);
  nodoIntv1->min = calcular_min(nodoIntv1);
  // return intv;
}

int hay_interseccion(Intervalo* intv1, Intervalo* intv2) {
  return intv1->der >= intv2->izq && intv1->izq <= intv2->der;
}

void buscar_repeticiones(Set nodo, Set aComparar, Pila* intvsAEliminar) {
  // puts("buscar_repeticiones: in");
  if (nodo) {
    if (hay_interseccion(nodo->intv, aComparar->intv) 
      || ((long)(aComparar->intv->der))+1 == nodo->intv->izq
      || ((long)(aComparar->intv->izq))-1 == nodo->intv->der) {
      // puts("Hola");
      // printf("Buscar repeticiones: BOOM entre "); imprimir_intv(aComparar->intv); printf(" y "); imprimir_intv(nodo->intv);
      // printf("before union_ints: %d:%d, max: %d, min: %d\n", aComparar->intv->izq, aComparar->intv->der, aComparar->max, aComparar->min);

      union_ints(aComparar, nodo->intv);
      // printf("after union_ints: %d:%d, max: %d, min: %d\n", aComparar->intv->izq, aComparar->intv->der, aComparar->max, aComparar->min);
      *intvsAEliminar = pila_apilar(*intvsAEliminar, nodo->intv);
      // aComparar = set_eliminar(aComparar, nodo->intv);
      // nodo = buscar_repeticiones(nodo, aComparar);

      // if (aComparar->intv->der >= nodo->intv->izq && aComparar->intv->der < nodo->intv->der-1) {
      //     nodo->intv->izq = aComparar->intv->der + 1;
      //     puts("buscar_repeticiones: all is well");
      // } else {
      //   if (nodo->intv->izq-1 == aComparar->intv->der)
      //     aComparar->intv->der == nodo->intv->der;
      //   puts("buscar_repeticiones: BOOM");
      //   nodo = set_eliminar(nodo, nodo->intv);
      // }
    }
    buscar_repeticiones(nodo->left, aComparar, intvsAEliminar);
    buscar_repeticiones(nodo->right, aComparar, intvsAEliminar);
  }
  // if (!nodo) puts("nodo NULL");
  // return aComparar;
}



/* set_insertar inserta un nodo en el lugar correspondiente del Set, y luego
realiza las rotaciones adecuadas si el árbol resultante está desbalanceado. */
Set set_insertar(Set nodo, Intervalo *intv) {
  // if (intv) {
  //   printf("Insertar ");
  //   imprimir_intv(intv);
  //   puts("");
  // }
  // printf("Quiere insertar %d:%d\n", intv->izq, intv->der);
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
  } else if (comparar_intervalos(intv, nodo->intv) < 0) { 
    // puts("comparar_intervalos < 0");
    if (((long)(intv->der))+1 >= nodo->intv->izq) {
      // puts("attach por lado izquierdo");
      Pila intvsAEliminar = pila_crear();
      // printf("buscar_repeticiones(nodo->left): %d:%d VS. %d:%d ---- %ld\n", nodo->intv->izq, nodo->intv->der, intv->izq, intv->der, ((long)(intv->der))+1);
      int nodoDer = nodo->intv->der;
      // printf("before union_ints: %d:%d, max: %d, min: %d\n", nodo->intv->izq, nodo->intv->der, nodo->max, nodo->min);
      union_ints(nodo, intv);
      nodo->max = calcular_max(nodo);
      nodo->min = calcular_min(nodo);
      // printf("after union_ints: %d:%d, max: %d, min: %d\n", nodo->intv->izq, nodo->intv->der, nodo->max, nodo->min);
      //nodo = 
      buscar_repeticiones(nodo->left, nodo, &intvsAEliminar);
      if (intv->der > nodoDer) {
        // nodo->right = 
        buscar_repeticiones(nodo->right, nodo, &intvsAEliminar);
      }
      // pila_imprimir(intvsAEliminar, imprimir_intv);
      while (!pila_es_vacia(intvsAEliminar)) {
        nodo = set_eliminar(nodo, pila_ultimo(intvsAEliminar));
        pila_desapilar(&intvsAEliminar);
      }
      pila_destruir(intvsAEliminar);
      // printf("NODO AFTER: %d:%d\n", nodo->intv->izq, nodo->intv->der);
      return nodo;
    } else {
    nodo->left = set_insertar(nodo->left, intv);
    }
  } else if (comparar_intervalos(intv, nodo->intv) > 0) {
    // puts("comparar_intervalos > 0");
    if (((long)(intv->izq))-1 <= nodo->intv->der) {
      // puts("attach por lado derecho");
      Pila intvsAEliminar = pila_crear();
      // printf("buscar_repeticiones(nodo->right): %d:%d VS. %d:%d\n", nodo->intv->izq, nodo->intv->der, intv->izq, intv->der);
      int nodoIzq = nodo->intv->izq;
      union_ints(nodo, intv);
      nodo->max = calcular_max(nodo);
      nodo->min = calcular_min(nodo);
      // nodo->right = 
      buscar_repeticiones(nodo->right, nodo, &intvsAEliminar);
      if (intv->izq < nodoIzq) {
        // nodo->left = 
        buscar_repeticiones(nodo->left, nodo, &intvsAEliminar);
      }
      // pila_imprimir(intvsAEliminar, imprimir_intv);
      while (!pila_es_vacia(intvsAEliminar)) {
        nodo = set_eliminar(nodo, pila_ultimo(intvsAEliminar));
        pila_desapilar(&intvsAEliminar);
      }
      pila_destruir(intvsAEliminar);
      // printf("NODO AFTER: %d:%d\n", nodo->intv->izq, nodo->intv->der);
      return nodo;
    } else {
    nodo->right = set_insertar(nodo->right, intv);
    }
  } else {
    // puts("comparar_intervalos == 0");
    /* Si el intervalo a insertar ya está en el árbol, no se inserta nada. */
    return nodo;
  }
  /* El programa entrará en esta sección de la función solo si se llamó a sí misma
  recursivamente en el subárbol izquierdo o derecho. Si el intervalo no está presente en el árbol, 
  la recursión desciende hasta alguna hoja del mismo donde se insertará el elemento, y luego 
  las alturas y máximos se irán actualizando desde abajo hacia arriba. */
  nodo->altura = 1 + maximo(itree_altura(nodo->left), itree_altura(nodo->right));
  nodo->max = calcular_max(nodo);
  nodo->min = calcular_min(nodo);
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


/* set_eliminar elimina un nodo si está presente en el Set, y luego
realiza las rotaciones correspondientes si el árbol resultante está desbalanceado. */
Set set_eliminar(Set nodo, Intervalo *intv) {
  if (!set_empty(nodo)) {
    if (nodo->intv->izq == intv->izq && nodo->intv->der == intv->der) { // Si el nodo actual es el nodo a eliminar.
      /* A continuación se distinguen los casos en los que el nodo tiene ninguno, uno o dos hijos no NULL.
      Los dos primeros casos son triviales, y en el caso donde tiene dos hijos no NULL,
      el nodo a eliminar se reemplaza por el mínimo nodo del subárbol derecho. */
      if (set_empty(nodo->left) && set_empty(nodo->right)) {
        free(nodo->intv);
        free(nodo);
        nodo = NULL;
      } else if (!set_empty(nodo->left) && !set_empty(nodo->right)) {
        Set minNodoDerecho = nodo->right;
        while (!set_empty(minNodoDerecho->left)) {
          minNodoDerecho = minNodoDerecho->left;
        }
        nodo->intv->izq = minNodoDerecho->intv->izq;
        nodo->intv->der = minNodoDerecho->intv->der;
        nodo->right = set_eliminar(nodo->right, minNodoDerecho->intv);
      } else {
        Set hijoNoNULL = (!set_empty(nodo->left)) ? nodo->left : nodo->right;
        free(nodo->intv);
        free(nodo);
        nodo = hijoNoNULL;
      }
    // } else if (comparar_intervalos(intv, nodo->intv) < 0) {
      /* Si el intervalo a eliminar es menor según el orden lexicográfico, si está
      en el árbol estará en el subárbol izquierdo, por lo que se llama a la función
      recursivamente sobre el subárbol izquierdo. */
      // nodo->left = set_eliminar(nodo->left, intv);
    } else {
      nodo->left = set_eliminar(nodo->left, intv);
      /* En caso contrario, si el intervalo a eliminar es un nodo del árbol, 
      estará en el subárbol derecho. */
      nodo->right = set_eliminar(nodo->right, intv);
    }
  }
  if (set_empty(nodo))
    return nodo;
  /* Se actualizan la altura y el máximo del árbol. */
  nodo->altura = 1 + maximo(itree_altura(nodo->left),itree_altura(nodo->right));
  nodo->max = calcular_max(nodo);
  nodo->min = calcular_min(nodo);
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


void set_imprimir_aux(Set tree, int max) {
  if (tree) {
    set_imprimir_aux(tree->left, max);
    imprimir_intv(tree->intv);//printf("(%d,%d)", tree->max, tree->min);
    if (tree->intv->der != max)
      printf(",");
    set_imprimir_aux(tree->right, max);
  }
}


void set_imprimir(Set tree) {
  if (tree) {
    set_imprimir_aux(tree, tree->max);
    puts("");
  }
  else
    printf("{}");
}


Set set_clonar_aux(Set resultado, Set nodo) {
  if (set_empty(nodo))
    return resultado;
  resultado = set_insertar(resultado, nodo->intv);
  resultado = set_clonar_aux(resultado, nodo->left);
  resultado = set_clonar_aux(resultado, nodo->right);
  return resultado;
}

Set set_clonar(Set arbol) {
    if (set_empty(arbol))
        return set_crear();
    Set resultado = set_crear();
    resultado = set_insertar(resultado, arbol->intv);
    set_clonar_aux(resultado, arbol);
    // printf("Despues de insertar ");imprimir_intv(arbol->intv);printf("(%d,%d)\n", arbol->max, arbol->min);
    // set_imprimir(arbol);
    return resultado;
}


Set unir_arboles(Set resultado, Set menorAltura) {
  if (set_empty(menorAltura))
    return resultado;
  resultado = set_insertar(resultado, menorAltura->intv);
  resultado = unir_arboles(resultado, menorAltura->left);
  resultado = unir_arboles(resultado, menorAltura->right);
  return resultado;
}

Set set_union(Set arbol1, Set arbol2) {
  if (set_empty(arbol1) || set_empty(arbol2)) {
    Set resultado = set_clonar((set_empty(arbol1)) ? arbol2 : arbol1);
    return resultado;
  }
  Set menorAltura = (arbol2->altura < arbol1->altura) ? arbol2 : arbol1;
  Set mayorAltura = (arbol2->altura < arbol1->altura) ? arbol1 : arbol2;
  Set resultado = set_clonar(mayorAltura);
  resultado = unir_arboles(resultado, menorAltura);
  return resultado;
}


Set generar_complemento(Set resultado, Set nodo, Intervalo* intv) {
  if (set_empty(nodo))
    return resultado;
  if (nodo->left && nodo->left->max+1 < nodo->intv->izq) {
    // puts("generar_comp: existe nodo-left");
    intv->izq = nodo->left->max+1;
    intv->der = nodo->intv->izq-1;
    resultado = set_insertar(resultado, intv);
    resultado = generar_complemento(resultado, nodo->left, intv);
  }
  if (nodo->right && nodo->right->min-1 > nodo->intv->der) {
    // puts("generar_comp: existe nodo-right");
    intv->izq = nodo->intv->der+1;
    intv->der = nodo->right->min-1;
    resultado = set_insertar(resultado, intv);
    resultado = generar_complemento(resultado, nodo->right, intv);
  }
  return resultado;
}

Set set_complemento(Set arbol) {
  Set resultado = set_crear();
  Intervalo* intv = malloc(sizeof(Intervalo));
  if (set_empty(arbol)) {
    // puts("Arbol vacío?");
    intv->izq = INT_MIN;
    intv->der = INT_MAX;
    resultado = set_insertar(resultado, intv);
  } else {
    if (arbol->min != INT_MIN) {
      // puts("Left side");
      intv->izq = INT_MIN;
      intv->der = arbol->min - 1;
      // printf("%d:%d\n", intv->izq, intv->der);
      resultado = set_insertar(resultado, intv);
    }
    if (arbol->max != INT_MAX) {
      // puts("Right side");
      intv->izq = arbol->max + 1;
      intv->der = INT_MAX;
      // printf("%d:%d\n", intv->izq, intv->der);
      resultado = set_insertar(resultado, intv);
    }
    // printf("---- min: %d max: %d ---SEMI TERMINADDO: ", arbol->min, arbol->max); set_imprimir(arbol);
    // printf("---- min: %d max: %d ---SEMI TERMINADDO: ", arbol->min, arbol->max); set_imprimir(resultado);
    // puts("");
    resultado = generar_complemento(resultado, arbol, intv);
    // printf("---- min: %d max: %d --- TERMINADDO: ", arbol->min, arbol->max); set_imprimir(resultado);
  }
  free(intv);
  return resultado;
}

int set_intersecar(Set arbol, Intervalo *intv, Intervalo* buffer) {
  if (!set_empty(arbol)) {
    printf("set_intersecar: %d:%d\n", intv->izq, intv->der);
    // if (intv->der < arbol->intv->izq || intv->izq > arbol->intv->der) { /* El intervalo no se interseca con la raíz. */
      if (!hay_interseccion(arbol->intv, intv)) {
      /* Si su subárbol izquierdo es no vacío y si el máximo del subárbol izquierdo es mayor o igual 
      al extremo izquierdo del intervalo, entonces es seguro que habrá intersección en ese subárbol. (*) */
      if (!set_empty(arbol->left) && intv->izq <= arbol->left->max && intv->der >= arbol->left->min)
        return set_intersecar(arbol->left, intv, buffer);
      /* (Si intv->izq es mayor que el ext. derecho de la raíz) Si su subárbol derecho es no vacío y si el máximo 
      del subárbol derecho es mayor o igual a intv->izq, entonces es posible que haya intersección en ese subárbol. */
      else if (intv->izq > arbol->intv->der && !set_empty(arbol->right) && intv->izq <= arbol->right->max && intv->der >= arbol->right->min)
        return set_intersecar(arbol->right, intv, buffer);
      else 
        return 0;
    } else { // En caso contrario, se interseca con el intervalo de la raíz.
      buffer->izq = (arbol->intv->izq > intv->izq) ? arbol->intv->izq : intv->izq;
      buffer->der = (arbol->intv->der < intv->der) ? arbol->intv->der : intv->der;
      return 1;
    }
  } else
    return 0;
}


Set generar_interseccion(Set resultado, Set menorRango, Set mayorRango, Intervalo* intv) {
  if (set_empty(mayorRango))
    return resultado;
  printf("generar intersección: %d:%d\n", mayorRango->intv->izq, mayorRango->intv->der);
  if (set_intersecar(menorRango, mayorRango->intv, intv)) {
    // set_imprimir(menorRango);
    printf("Intersección entre %d:%d y %d:%d -> %d:%d\n", menorRango->intv->izq, menorRango->intv->der, mayorRango->intv->izq, mayorRango->intv->der, intv->izq, intv->der);
    resultado = set_insertar(resultado, intv);
  }
  resultado = generar_interseccion(resultado, menorRango, mayorRango->left, intv);
  resultado = generar_interseccion(resultado, menorRango, mayorRango->right, intv);
  return resultado;
  
}

Set set_interseccion(Set arbol1, Set arbol2) {
  if (set_empty(arbol1) || set_empty(arbol2))
    return NULL;
  Set menorRango = ((long)(arbol1->max) - arbol1->min > (long)(arbol2->max) - arbol2->min) ? arbol2 : arbol1;
  Set mayorRango = ((long)(arbol1->max) - arbol1->min > (long)(arbol2->max) - arbol2->min) ? arbol1 : arbol2;
  printf("Intersecar 1. "); set_imprimir(menorRango); printf("con 2. "); set_imprimir(mayorRango); puts("");
  // printf("Mayor rango min: %d - max: %d\n", mayorRango->min, mayorRango->max); 
  // printf("Menor rango min: %d - max: %d\n", menorRango->min, menorRango->max); 
  // printf("Mayor rango: "); set_imprimir(mayorRango); puts("");
  // printf("Menor rango: "); set_imprimir(menorRango);  puts("");
  Intervalo* intv = malloc(sizeof(Intervalo));
  Set resultado = set_crear();
  resultado = generar_interseccion(resultado, menorRango, mayorRango, intv);

  free(intv);
  return resultado;
}

// resultado = arbol1 - arbol2
Set set_resta(Set arbol1, Set arbol2) {
  Set resultado = set_crear();
  resultado = set_complemento(arbol2);
  resultado = set_interseccion(arbol1, resultado);
  return resultado;
}



// TO DELETE
Set set_crear_sucesion(FunGeneradora generar, Numeros direccion, int posInicial) {
  long elem = posInicial;
  Set resultado = set_crear();
  Intervalo* intv = malloc(sizeof(Intervalo));
  // resultado = set_insertar(resultado, posInicial);
  while (elem < INT_MAX) {
    intv->izq = (direccion == Negativos) ? -elem : elem;
    intv->der = (direccion == Negativos) ? -elem : elem;
    resultado = set_insertar(resultado, intv);
    if (direccion == Ambos) {
      intv->izq = -elem;
      intv->der = -elem;
      resultado = set_insertar(resultado, intv);
    }
    elem = generar(elem);
  }
  free(intv);
  return resultado;
}