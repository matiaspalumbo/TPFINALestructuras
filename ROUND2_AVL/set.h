#ifndef __SET_H__
#define __SET_H__


/* Se representan conjuntos (Sets) de enteros mediante árboles AVL de intervalos. */

typedef struct {
  int izq; // Extremo izquierdo.
  int der; // Extremo derecho.
} Intervalo;

typedef struct _SetNodo {
  Intervalo* intv;
  int max, min, altura; // Extremo derecho máximo, izquierdo mínimo y altura del nodo, respectivamente.
  struct _SetNodo *right; // Hijo derecho.
  struct _SetNodo *left; // Hijo izquierdo.
} SetNodo;

typedef SetNodo* Set;

typedef Set (*OperacionBinaria)(Set, Set);


/* Crea un conjunto vacío. */
Set set_crear();

/* Destruye un conjunto. */
void set_destruir(Set nodo);

/* Verifica si el conjunto dado es vacío. */
int set_empty(Set nodo);

/* Agrega un intervalo al conjunto dado. */
Set set_insertar(Set nodo, Intervalo *intv);

/* Elimina (si existe) un nodo con el intervalo dado. */
Set set_eliminar(Set nodo, Intervalo *intv);

/* Devuelve una copia del set dado. */
Set set_clonar(Set set);

/* Devuelve la unión de los conjuntos dados. */
Set set_union(Set set1, Set set2);

/* Devuelve el complemento del conjunto dado. */
Set set_complemento(Set set);

/* Devuelve la intersección de los conjuntos dados. */
Set set_interseccion(Set set1, Set set2);

/* Devuelve la resta de set1 con set2 (set1 \ set2). */
Set set_resta(Set set1, Set set2);


/* Imprime el conjunto dado en orden, sus elementos separados por ",". */
void set_imprimir(Set set);


#endif /* __SET_H__ */
