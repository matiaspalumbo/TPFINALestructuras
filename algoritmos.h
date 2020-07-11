#ifndef __ALGORITMOS_H__
#define __ALGORITMOS_H__

#include <stddef.h>
#include "gdclist.h"


/* Archivo de cabecera de la implementación de los algoritmos de ordenación 
Selection Sort, Insertion Sort y Merge Sort. */


/* Declaración de un puntero a una función que toma como argumentos dos punteros
a void y retorna un entero. Las funciones de este tipo se utilizarán para comparar 
dos valores según un criterio, y devuelve un número negativo si dato1 < dato2, 0 si 
dato1 == dato2, y un número positivo si dato1 > dato2. */
typedef int (*FuncionComparadora) (void *dato1, void *dato2);

/* Declaración de un puntero a una función que toma como argumento una GList y 
una FuncionComparadora y retorna una GList. Las funciones de este tipo se utilizarán 
para ordenar la lista dada. */
typedef GList (*AlgoritmoSorting) (GList lista, FuncionComparadora comparar);

// Ordena una lista utilizando Merge Sort.
GList merge_sort(GList lista, FuncionComparadora comparar);


#endif /* __ALGORITMOS_H */