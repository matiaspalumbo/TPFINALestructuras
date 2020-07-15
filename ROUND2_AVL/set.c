#include <stdlib.h>
#include <stdio.h>
#include "set.h"
#include "cola.h"
#include "pila.h"
#include "limits.h"

/* Archivo con la implementación de Árboles de Intervalos AVL. */


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


long comparar_intervalos(Intervalo *intv1, Intervalo *intv2) {
  return (long)intv1->izq - (long)intv2->izq;
  // return (intv1->izq == intv2->izq) ? intv1->der - intv2->der : intv1->izq - intv2->izq;
}


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
  // return intv;
}

Set buscar_repeticiones(Set nodo, Set aComparar) {
  // puts("buscar_repeticiones: in");
  if (nodo) {
    nodo->left = buscar_repeticiones(nodo->left, aComparar);
    nodo->right = buscar_repeticiones(nodo->right, aComparar);
    if (nodo->intv->der >= aComparar->intv->izq && nodo->intv->izq <= aComparar->intv->der) {
    // if ((nodo->intv->izq <= aComparar->intv->izq && aComparar->intv->izq <= nodo->intv->der)
    // || (nodo->intv->izq <= aComparar->intv->der && aComparar->intv->der <= nodo->intv->der)) {
      // puts("buscar_repeticiones: BOOM");
      nodo = set_eliminar(nodo, nodo->intv);
    }
  }
  // if (!nodo) puts("nodo NULL");
  return nodo;
}

/* set_insertar inserta un nodo en el lugar correspondiente del Set, y luego
realiza las rotaciones adecuadas si el árbol resultante está desbalanceado. */
Set set_insertar(Set nodo, Intervalo *intv) {
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
      if (((long)(intv->der))+1 >= nodo->intv->izq) { // se rompe con INT_MIN Y INT_MAX
        // printf("buscar_repeticiones(nodo->left): %d:%d VS. %d:%d ---- %ld\n", nodo->intv->izq, nodo->intv->der, intv->izq, intv->der, ((long)(intv->der))+1);
        union_ints(nodo, intv);
        nodo->max = calcular_max(nodo);
        nodo->min = calcular_min(nodo);
        nodo->left = buscar_repeticiones(nodo->left, nodo);
        // printf("NODO AFTER: %d:%d\n", nodo->intv->izq, nodo->intv->der);
        return nodo;
      }
    nodo->left = set_insertar(nodo->left, intv);
    } else if (comparar_intervalos(intv, nodo->intv) > 0) {
      if (((long)(intv->izq))-1 <= nodo->intv->der) {
        // printf("buscar_repeticiones(nodo->right): %d:%d VS. %d:%d\n", nodo->intv->izq, nodo->intv->der, intv->izq, intv->der);
        union_ints(nodo, intv);
        nodo->max = calcular_max(nodo);
        nodo->min = calcular_min(nodo);
        nodo->right = buscar_repeticiones(nodo->right, nodo);
        // printf("NODO AFTER: %d:%d\n", nodo->intv->izq, nodo->intv->der);
        return nodo;
      }
    nodo->right = set_insertar(nodo->right, intv);
  } else {
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
    if (comparar_intervalos(intv, nodo->intv) == 0) { // Si el nodo actual es el nodo a eliminar.
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
    } else if (comparar_intervalos(intv, nodo->intv) < 0) {
      /* Si el intervalo a eliminar es menor según el orden lexicográfico, si está
      en el árbol estará en el subárbol izquierdo, por lo que se llama a la función
      recursivamente sobre el subárbol izquierdo. */
      nodo->left = set_eliminar(nodo->left, intv);
    } else {
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


void imprimir_intervalo(Set nodo) {
  if (!set_empty(nodo)) printf("[%d, %d] ", nodo->intv->izq, nodo->intv->der);
}


void set_recorrer_dfs(Set arbol, FuncionVisitante visit) {
  if (!set_empty(arbol)) {
    set_recorrer_dfs(arbol->left, visit);
    visit(arbol);
    set_recorrer_dfs(arbol->right, visit);
  }
}


/* La función set_recorrer_bfs utiliza colas como estructura auxiliar
para almacenar los nodos a visitar. */
void set_recorrer_bfs(Set arbol, FuncionVisitante visit) {
  if (!set_empty(arbol)) {
    /* Crea una cola con la raiz del árbol. */
    Cola queue = cola_crear();
    cola_encolar(queue, arbol);
    SetNodo *temp;
    while (!cola_es_vacia(queue)) {
      /* Mientras la cola no sea vacía, visito el primer elemento y lo elimino de la cola. */
      temp = cola_primero(queue);
      cola_desencolar(queue);
      visit(temp);
      /* Luego agrego a la cola los hijos de ese nodo, en el orden que quiero que sean visitados. */
      if (!set_empty(temp->left)) cola_encolar(queue, temp->left);
      if (!set_empty(temp->right)) cola_encolar(queue, temp->right);
    }
    cola_destruir(queue);
  }
}

void imprimir_intv(void* intv) {
  if (((Intervalo*)intv)->izq == ((Intervalo*)intv)->der) {
    printf("%d", ((Intervalo*)intv)->izq);
  } else {
    printf("%d:%d", ((Intervalo*)intv)->izq, ((Intervalo*)intv)->der);
  }
}

void set_imprimir_aux(Set tree, int max) {
  if (tree) {
    set_imprimir_aux(tree->left, max);
    imprimir_intv(tree->intv);
    if (tree->intv->der != max)
      printf(",");
    set_imprimir_aux(tree->right, max);
  }
}


void set_imprimir(Set tree) {
  if (tree)
    set_imprimir_aux(tree, tree->max);
  else
    printf("{}");
}


// void set_imprimir(Set arbol) {
//   if (!set_empty(arbol)) {
//     Pila pila = pila_crear();
//     Set nodoActual = arbol;
//     int terminar = 0;
//     while (!terminar) {
//       while (nodoActual) {
//         pila = pila_apilar(pila, nodoActual);
//         nodoActual = nodoActual->left;
//       }
//       nodoActual = pila_ultimo(pila);
//       pila_desapilar(&pila);
//       imprimir_intv(nodoActual->intv);
//       nodoActual = nodoActual->right;
//       if (pila_es_vacia(pila) && !nodoActual) {
//         terminar = 1;
//         puts("");
//       } else printf(",");
//     }
//     pila_destruir(pila);
//   } else puts("{}");
// }


Set set_clonar(Set arbol) {
    if (set_empty(arbol))
        return NULL;
    Set nodo = set_crear();
    nodo = set_insertar(nodo, arbol->intv);
    nodo->left = set_clonar(arbol->left);
    nodo->right = set_clonar(arbol->right);
    /* Return root of cloned tree */
    return nodo;
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
    if (intv->der < arbol->intv->izq || intv->izq > arbol->intv->der) { /* El intervalo no se interseca con la raíz. */
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
    set_imprimir(menorRango);
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