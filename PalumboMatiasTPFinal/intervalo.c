#include "intervalo.h"
#include <stdlib.h>
#include <stdio.h>


Intervalo* intv_crear() {
  Intervalo* intv = malloc(sizeof(Intervalo));
  return intv;
}


void intv_destruir(Intervalo* intv) {
  free(intv);
}


/* Devuelve 0 si intvA está contenido en intvB. Caso contrario,
los compara de acuerdo al orden lexicográfico. */
long intv_comparar(Intervalo *intvA, Intervalo *intvB) {
  if (intvA->izq >= intvB->izq && intvA->der <= intvB->der)
    return 0;
  else
    return  (intvA->izq == intvB->izq) ? ((long)intvA->der - intvB->der) : ((long)intvA->izq - intvB->izq);
}


/* Verifica si dos intervalos se intersecan. */
int intv_hay_interseccion(Intervalo* intv1, Intervalo* intv2) {
  return intv1->der >= intv2->izq && intv1->izq <= intv2->der;
}


/* Imprime un intervalo. Si los extremos son iguales, imprime 
solo un extremo, y si no, imprime ambos separados por ":". */
void intv_imprimir(void* intv) {
  if (((Intervalo*)intv)->izq == ((Intervalo*)intv)->der)
    printf("%d", ((Intervalo*)intv)->izq);
  else
    printf("%d:%d", ((Intervalo*)intv)->izq, ((Intervalo*)intv)->der);
}