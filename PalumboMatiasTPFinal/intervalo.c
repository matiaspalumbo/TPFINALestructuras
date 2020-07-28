#include "intervalo.h"
#include <stdlib.h>
#include <stdio.h>

/* Archivo con la implementación de intervalos enteros. */


Intervalo* intv_crear() {
  Intervalo* intv = malloc(sizeof(Intervalo));
  return intv;
}


void intv_destruir(Intervalo* intv) {
  free(intv);
}


long intv_comparar(Intervalo *intvA, Intervalo *intvB) {
  if (intvA->izq >= intvB->izq && intvA->der <= intvB->der)
    return 0;
  else
    return  (intvA->izq == intvB->izq) ? ((long)intvA->der - intvB->der) : ((long)intvA->izq - intvB->izq);
}


int intv_hay_interseccion(Intervalo* intv1, Intervalo* intv2) {
  return intv1->der >= intv2->izq && intv1->izq <= intv2->der;
}


void intv_imprimir(void* intv) {
  if (((Intervalo*)intv)->izq == ((Intervalo*)intv)->der)
    printf("%d", ((Intervalo*)intv)->izq);
  else
    printf("%d:%d", ((Intervalo*)intv)->izq, ((Intervalo*)intv)->der);
}