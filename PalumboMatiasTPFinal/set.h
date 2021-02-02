#ifndef __SET_H__
#define __SET_H__

#include "itree.h"
#include "intervalo.h"

/* Archivo de cabecera de la implementación de conjuntos de números enteros. */


/* Los conjuntos se representan mediante árboles AVL de intervalos. */
typedef ITree Set;

typedef Set (*OperacionBinaria)(Set, Set);


/* Crea un conjunto vacío. */
Set set_crear();

/* Destruye un conjunto. */
void set_destruir(Set set);

/* Verifica si el conjunto dado es vacío. */
int set_empty(Set set);

/* Agrega un intervalo al conjunto dado. */
Set set_insertar(Set set, Intervalo *intv);

/* Imprime el conjunto dado en orden, sus elementos separados por ",". */
void set_imprimir(Set set);

/* Devuelve una copia del conjunto dado. */
Set set_clonar(Set set);

/* Devuelve la unión de los conjuntos dados. */
Set set_union(Set set1, Set set2);

/* Devuelve el complemento del conjunto dado. */
Set set_complemento(Set set);

/* Devuelve la intersección de los conjuntos dados. */
Set set_interseccion(Set set1, Set set2);

/* Devuelve la resta de set1 con set2 (set1 \ set2). */
Set set_resta(Set set1, Set set2);


#endif /* __SET_H__ */

