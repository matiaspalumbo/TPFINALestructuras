#include <stdlib.h>
#include <stdio.h>
#include "itree.h"
#include "cola.h"
#include "pila.h"
#include "limits.h"

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


int maximo(int n1, int n2) {
  return n1 < n2 ? n2 : n1;
}

int minimo(int n1, int n2) {
  return n1 < n2 ? n1 : n2;
}

/* Calcula la altura de un ITree luego de insertar un nodo. */
int calcular_altura(ITree nodo) {
  return 1 + maximo(itree_altura(nodo->right), itree_altura(nodo->left));
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


long comparar_intervalos(Intervalo *intv1, Intervalo *intv2) {
  return (long)intv1->izq - (long)intv2->izq;
  // return (intv1->izq == intv2->izq) ? intv1->der - intv2->der : intv1->izq - intv2->izq;
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
  /* Se actualizan las alturas y máximos de los nodos rotados. */
  nodo->altura = 1 + maximo(itree_altura(nodo->right), (itree_altura(nodo->left)));
  nuevaRaiz->altura = 1 + maximo(itree_altura(nuevaRaiz->right), (itree_altura(nuevaRaiz->left)));
  nodo->max = calcular_max(nodo);
  nuevaRaiz->max = calcular_max(nuevaRaiz);
  nodo->min = calcular_min(nodo);
  nuevaRaiz->min = calcular_min(nuevaRaiz);
  return nuevaRaiz;
}

void union_ints(ITNodo* nodoIntv1, Intervalo* intv2) { // Une dos intervalos que se intersecan.
  // Intervalo* intv = malloc(sizeof(Intervalo));
  nodoIntv1->intv->izq = (nodoIntv1->intv->izq < intv2->izq) ? nodoIntv1->intv->izq : intv2->izq;
  nodoIntv1->intv->der = (nodoIntv1->intv->der > intv2->der) ? nodoIntv1->intv->der : intv2->der;
  // return intv;
}

ITree buscar_repeticiones(ITree nodo, ITree aComparar) {
  // puts("buscar_repeticiones: in");
  if (nodo) {
    nodo->left = buscar_repeticiones(nodo->left, aComparar);
    nodo->right = buscar_repeticiones(nodo->right, aComparar);
    if ((nodo->intv->izq <= aComparar->intv->izq && aComparar->intv->izq <= nodo->intv->der)
    || (nodo->intv->izq <= aComparar->intv->der && aComparar->intv->der <= nodo->intv->der)) {
      // puts("buscar_repeticiones: BOOM");
      nodo = itree_eliminar(nodo, nodo->intv);
    }
  }
  // if (!nodo) puts("nodo NULL");
  return nodo;
}

/* itree_insertar inserta un nodo en el lugar correspondiente del ITree, y luego
realiza las rotaciones adecuadas si el árbol resultante está desbalanceado. */
ITree itree_insertar(ITree nodo, Intervalo *intv) {
  // printf("Quiere insertar %d:%d\n", intv->izq, intv->der);
  if (itree_empty(nodo)) { /* Si el nodo es vacío, se debe insertar aquí. */
    nodo = malloc(sizeof(ITNodo));
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
    nodo->left = itree_insertar(nodo->left, intv);
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
    nodo->right = itree_insertar(nodo->right, intv);
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


void imprimir_intervalo(ITree nodo) {
  if (!itree_empty(nodo)) printf("[%d, %d] ", nodo->intv->izq, nodo->intv->der);
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

void itree_imprimir(ITree tree) {
  if (tree) {
    itree_imprimir(tree->left);
    printf("%d:%d, ", tree->intv->izq, tree->intv->der);
    itree_imprimir(tree->right);
  }
}

// void imprimir_intv(void* intv) {
//   if (((Intervalo*)intv)->izq == ((Intervalo*)intv)->der) {
//     printf("%d", ((Intervalo*)intv)->izq);
//   } else {
//     printf("%d:%d", ((Intervalo*)intv)->izq, ((Intervalo*)intv)->der);
//   }
// }

// void itree_imprimir(ITree arbol) {
//   if (itree_empty(arbol)) {
//     Pila pila = pila_crear();
//     ITree nodoActual = arbol;
//     int terminar = 0;
//     while (!terminar) {
//       while (nodoActual) {
//         pila = pila_apilar(pila, nodoActual);
//         nodoActual = nodoActual->left;
//       }
//       pila_imprimir(pila, imprimir_intv);
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


ITree itree_clonar(ITree arbol){
    if (itree_empty(arbol))
        return NULL;
    ITree nodo = itree_crear();
    nodo = itree_insertar(nodo, arbol->intv);
    nodo->left = itree_clonar(arbol->left);
    nodo->right = itree_clonar(arbol->right);
    /* Return root of cloned tree */
    return nodo;
}


ITree unir_arboles(ITree resultado, ITree menorAltura) {
  if (itree_empty(menorAltura))
    return resultado;
  resultado = itree_insertar(resultado, menorAltura->intv);
  resultado = unir_arboles(resultado, menorAltura->left);
  resultado = unir_arboles(resultado, menorAltura->right);
  return resultado;
}

ITree itree_union(ITree arbol1, ITree arbol2) {
  ITree menorAltura = (arbol2->altura < arbol1->altura) ? arbol2 : arbol1;
  ITree mayorAltura = (arbol2->altura < arbol1->altura) ? arbol1 : arbol2;
  ITree resultado = itree_clonar(mayorAltura);
  resultado = unir_arboles(resultado, menorAltura);
  return resultado;
}


ITree generar_complemento(ITree resultado, ITree nodo, Intervalo* intv) {
  if (itree_empty(nodo))
    return resultado;
  if (nodo->left && nodo->left->max+1 < nodo->intv->izq) {
    // puts("generar_comp: existe nodo-left");
    intv->izq = nodo->left->max+1;
    intv->der = nodo->intv->izq-1;
    resultado = itree_insertar(resultado, intv);
    resultado = generar_complemento(resultado, nodo->left, intv);
  }
  if (nodo->right && nodo->right->min-1 > nodo->intv->der) {
    // puts("generar_comp: existe nodo-right");
    intv->izq = nodo->intv->der+1;
    intv->der = nodo->right->min-1;
    resultado = itree_insertar(resultado, intv);
    resultado = generar_complemento(resultado, nodo->right, intv);
  }
  return resultado;
}

ITree itree_complemento(ITree arbol) {
  ITree resultado = itree_crear();
  Intervalo* intv = malloc(sizeof(Intervalo));
  if (itree_empty(arbol)) {
    // puts("Arbol vacío?");
    intv->izq = INT_MIN;
    intv->der = INT_MAX;
    resultado = itree_insertar(resultado, intv);
  } else {
    if (arbol->min != INT_MIN) {
      // puts("Left side");
      intv->izq = INT_MIN;
      intv->der = arbol->min - 1;
      // printf("%d:%d\n", intv->izq, intv->der);
      resultado = itree_insertar(resultado, intv);
    }
    if (arbol->max != INT_MAX) {
      // puts("Right side");
      intv->izq = arbol->max + 1;
      intv->der = INT_MAX;
      // printf("%d:%d\n", intv->izq, intv->der);
      resultado = itree_insertar(resultado, intv);
    }
    // printf("---- min: %d max: %d ---SEMI TERMINADDO: ", arbol->min, arbol->max); itree_imprimir(arbol);
    // printf("---- min: %d max: %d ---SEMI TERMINADDO: ", arbol->min, arbol->max); itree_imprimir(resultado);
    // puts("");
    resultado = generar_complemento(resultado, arbol, intv);
    // printf("---- min: %d max: %d --- TERMINADDO: ", arbol->min, arbol->max); itree_imprimir(resultado);
  }
  free(intv);
  return resultado;
}

int itree_intersecar(ITree arbol, Intervalo *intv, Intervalo* buffer) {
  if (!itree_empty(arbol)) {
    if (intv->der < arbol->intv->izq || intv->izq > arbol->intv->der) { /* El intervalo no se interseca con la raíz. */
      /* Si su subárbol izquierdo es no vacío y si el máximo del subárbol izquierdo es mayor o igual 
      al extremo izquierdo del intervalo, entonces es seguro que habrá intersección en ese subárbol. (*) */
      if (!itree_empty(arbol->left) && intv->izq <= arbol->left->max)
        return itree_intersecar(arbol->left, intv, buffer);
      /* (Si intv->izq es mayor que el ext. derecho de la raíz) Si su subárbol derecho es no vacío y si el máximo 
      del subárbol derecho es mayor o igual a intv->izq, entonces es posible que haya intersección en ese subárbol. */
      else if (intv->izq > arbol->intv->der && !itree_empty(arbol->right) && intv->izq <= arbol->right->max)
        return itree_intersecar(arbol->right, intv, buffer);
      else 
        return 0;
    } else {// En caso contrario, se interseca con el intervalo de la raíz.
      buffer->izq = (arbol->intv->izq > intv->izq) ? arbol->intv->izq : intv->izq;
      buffer->der = (arbol->intv->der < intv->der) ? arbol->intv->der : intv->der;
      return 1;
    }
  } else
    return 0;
}


ITree generar_interseccion(ITree resultado, ITree menorRango, ITree mayorRango, Intervalo* intv) {
  if (itree_empty(mayorRango))
    return resultado;
  if (itree_intersecar(menorRango, mayorRango->intv, intv)) {
    printf("Intersección entre %d:%d y %d:%d -> %d:%d\n", mayorRango->intv->izq, mayorRango->intv->der, menorRango->intv->izq, menorRango->intv->der, intv->izq, intv->der);
    resultado = itree_insertar(resultado, intv);
  }
  resultado = generar_interseccion(resultado, menorRango, mayorRango->left, intv);
  resultado = generar_interseccion(resultado, menorRango, mayorRango->right, intv);
  return resultado;
  
}

ITree itree_interseccion(ITree arbol1, ITree arbol2) {
  if (itree_empty(arbol1) || itree_empty(arbol2))
    return NULL;
  ITree menorRango = ((long)(arbol1->max) - arbol1->min > (long)(arbol2->max) - arbol2->min) ? arbol2 : arbol1;
  ITree mayorRango = ((long)(arbol1->max) - arbol1->min > (long)(arbol2->max) - arbol2->min) ? arbol1 : arbol2;
  // printf("Mayor rango min: %d - max: %d\n", mayorRango->min, mayorRango->max); 
  // printf("Menor rango min: %d - max: %d\n", menorRango->min, menorRango->max); 
  // printf("Mayor rango: "); itree_imprimir(mayorRango); puts("");
  // printf("Menor rango: "); itree_imprimir(menorRango);  puts("");
  Intervalo* intv = malloc(sizeof(Intervalo));
  ITree resultado = itree_crear();
  resultado = generar_interseccion(resultado, menorRango, mayorRango, intv);

  free(intv);
  return resultado;
}


// TO DELETE
ITree itree_crear_sucesion(FunGeneradora generar, Numeros direccion, int posInicial) {
  long elem = posInicial;
  ITree resultado = itree_crear();
  Intervalo* intv = malloc(sizeof(Intervalo));
  // resultado = itree_insertar(resultado, posInicial);
  while (elem < INT_MAX) {
    intv->izq = (direccion == Negativos) ? -elem : elem;
    intv->der = (direccion == Negativos) ? -elem : elem;
    resultado = itree_insertar(resultado, intv);
    if (direccion == Ambos) {
      intv->izq = -elem;
      intv->der = -elem;
      resultado = itree_insertar(resultado, intv);
    }
    elem = generar(elem);
  }
  free(intv);
  return resultado;
}