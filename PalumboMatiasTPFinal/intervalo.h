#ifndef __INTV_H__
#define __INTV_H__


/* Se representan intervalos enteros. */

typedef struct {
  int izq; // Extremo izquierdo.
  int der; // Extremo derecho.
} Intervalo;


Intervalo* intv_crear();

void intv_destruir(Intervalo* intv);

long intv_comparar(Intervalo *intvA, Intervalo *intvB);

int intv_hay_interseccion(Intervalo* intv1, Intervalo* intv2);

void intv_imprimir(void* intv);


#endif  /* __INTV_H__ */